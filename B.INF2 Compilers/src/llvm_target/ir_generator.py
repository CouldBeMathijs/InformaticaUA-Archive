from src.parser.AST.Node import *
from src.parser.AST.Visitor import AstVisitor
from llvmlite import ir, binding
from src.llvm_target.to_llvmlite import LLVMTypeMap
from copy import deepcopy


_original_instruction_str = ir.Instruction.__str__


def _instruction_str_with_comments(self):
    base_str = _original_instruction_str(self)

    # Check if we attached an inline comment to this python object
    if hasattr(self, "_inline_comment") and self._inline_comment:
        # If it's a list (e.g. multiple comments), join them
        if isinstance(self._inline_comment, list):
            comment_text = " ".join(self._inline_comment)
        else:
            comment_text = str(self._inline_comment)

        # Clean up newlines so it doesn't break the LLVM IR formatting
        comment_text = comment_text.replace("\n", " ").replace("\0a", " ")

        # Append the native LLVM inline comment
        return f"{base_str}  ; {comment_text}"

    return base_str


ir.Instruction.__str__ = _instruction_str_with_comments


class VLAMarker:
    """Wrapper to mark a pointer as coming from a Variable Length Array (VLA) allocation."""

    def __init__(self, ptr):
        self.ptr = ptr

    def __getattr__(self, name):
        # Delegate attribute access to the wrapped pointer
        return getattr(self.ptr, name)


class IRGenerator(AstVisitor):
    def __init__(self, triple=None):
        self.module = ir.Module(name="compilers_uantwerp")
        if triple:
            binding.initialize_all_targets()
            target_triple = triple
        else:
            binding.initialize_native_target()
            binding.initialize_native_asmprinter()
            target_triple = binding.get_default_triple()
        target = binding.Target.from_triple(target_triple)
        target_machine = target.create_target_machine()

        self.module.triple = target_triple
        self.module.data_layout = str(target_machine.target_data)

        self.builder = None
        self.struct_scopes = [{}]
        self.typedef_scopes = [{}]
        self.struct_info_by_type_id = {}
        self._struct_uid = 0
        self.type_map = LLVMTypeMap(
            struct_resolver=self._resolve_struct_type,
            typedef_resolver=self._resolve_typedef_type,
        )
        self.global_symbols = {}
        self.local_scopes = []
        self.loop_blocks = []
        self._declare_builtin_functions()

    def _enter_local_scope(self):
        self.local_scopes.append({})
        self.struct_scopes.append({})
        self.typedef_scopes.append({})

    def _exit_local_scope(self):
        if self.local_scopes:
            self.local_scopes.pop()
        if len(self.struct_scopes) > 1:
            self.struct_scopes.pop()
        if len(self.typedef_scopes) > 1:
            self.typedef_scopes.pop()

    def _define_symbol(self, name, ptr, is_global=False):
        if is_global:
            self.global_symbols[name] = ptr
            return

        if not self.local_scopes:
            self._enter_local_scope()
        self.local_scopes[-1][name] = ptr

    def _lookup_symbol(self, name):
        for scope in reversed(self.local_scopes):
            if name in scope:
                return scope[name]
        return self.global_symbols.get(name)

    def _current_struct_scope(self):
        if self.local_scopes:
            return self.struct_scopes[-1]
        return self.struct_scopes[0]

    def _current_typedef_scope(self):
        if self.local_scopes:
            return self.typedef_scopes[-1]
        return self.typedef_scopes[0]

    def _lookup_struct_info(self, name):
        for scope in reversed(self.struct_scopes):
            if name in scope:
                return scope[name]
        return None

    def _lookup_typedef(self, name):
        for scope in reversed(self.typedef_scopes):
            if name in scope:
                return scope[name]
        return None

    def _resolve_typedef_type(self, type_node, seen=None):
        if not isinstance(type_node, TypeNode):
            return type_node

        alias_name = getattr(type_node, "alias_name", None)
        if not alias_name:
            return type_node

        if seen is None:
            seen = set()
        if alias_name in seen:
            return type_node

        target = self._lookup_typedef(alias_name)
        if target is None:
            return type_node

        resolved_base = self._resolve_typedef_type(target, seen | {alias_name})
        if not isinstance(resolved_base, TypeNode):
            return type_node

        resolved = deepcopy(resolved_base)
        resolved.is_const = getattr(resolved_base, "is_const", False) or getattr(
            type_node, "is_const", False
        )
        resolved.ptr_const_quals = list(
            getattr(resolved_base, "ptr_const_quals", [])
        ) + list(getattr(type_node, "ptr_const_quals", []))
        resolved.ptr_depth = getattr(resolved_base, "ptr_depth", 0) + getattr(
            type_node, "ptr_depth", 0
        )
        if getattr(type_node, "array_dimensions", None) is not None:
            resolved.array_dimensions = type_node.array_dimensions
        resolved.alias_name = None
        return resolved
        # Gebruik id(self) zodat de naam uniek is per IRGenerator run!

    def _new_struct_type(self, name):
        llvm_name = f"struct.{name}.{id(self)}.{self._struct_uid}"
        self._struct_uid += 1
        return self.module.context.get_identified_type(llvm_name)

    def _resolve_struct_type(self, name):
        info = self._lookup_struct_info(name)
        if info is not None:
            return info["llvm_type"]

        info = {
            "llvm_type": self._new_struct_type(name),
            "field_indices": {},
            "field_types": {},
            "defined": False,
        }
        self._current_struct_scope()[name] = info
        return info["llvm_type"]

    def _register_struct(self, node):
        if not getattr(node, "name", None):
            return None

        scope = self._current_struct_scope()
        info = scope.get(node.name)
        if info is None:
            info = {
                "llvm_type": self._new_struct_type(node.name),
                "field_indices": {},
                "field_types": {},
                "defined": False,
                "is_union": getattr(node, "is_union", False),
            }
            scope[node.name] = info

        if getattr(node, "members", None):
            field_indices = {}
            field_types = {}
            llvm_fields = []
            for idx, member in enumerate(node.members):
                field_indices[member.name] = idx
                field_types[member.name] = member.datatype
                llvm_fields.append(self.type_map.get_llvm_type(member.datatype))

            if not info["defined"]:
                if getattr(info["llvm_type"], "is_opaque", True):
                    if info.get("is_union", False) and llvm_fields:
                        # For unions, we only use the first field for the body.
                        # Memory is overlapping, access is handled via bitcast.
                        info["llvm_type"].set_body(llvm_fields[0])
                    else:
                        info["llvm_type"].set_body(*llvm_fields)
                info["defined"] = True

            info["field_indices"] = field_indices
            info["field_types"] = field_types
            self.struct_info_by_type_id[id(info["llvm_type"])] = info

        return info

    def _declare_builtin_functions(self):
        printf_type = ir.FunctionType(
            ir.IntType(32), [ir.IntType(8).as_pointer()], var_arg=True
        )
        ir.Function(self.module, printf_type, name="printf")

        scanf_type = ir.FunctionType(
            ir.IntType(32), [ir.IntType(8).as_pointer()], var_arg=True
        )
        ir.Function(self.module, scanf_type, name="scanf")

        void_ptr = ir.IntType(8).as_pointer()
        i32 = ir.IntType(32)
        void_type = ir.VoidType()

        malloc_type = ir.FunctionType(void_ptr, [i32])
        ir.Function(self.module, malloc_type, name="malloc")

        calloc_type = ir.FunctionType(void_ptr, [i32, i32])
        ir.Function(self.module, calloc_type, name="calloc")

        free_type = ir.FunctionType(void_type, [void_ptr])
        ir.Function(self.module, free_type, name="free")

        realloc_type = ir.FunctionType(void_ptr, [void_ptr, i32])
        ir.Function(self.module, realloc_type, name="realloc")

        file_struct = self._resolve_struct_type("_IO_FILE")
        self.typedef_scopes[0]["FILE"] = TypeNode(
            0, 0, BaseType.STRUCT, struct_name="_IO_FILE"
        )
        file_ptr = file_struct.as_pointer()

        fopen_type = ir.FunctionType(file_ptr, [void_ptr, void_ptr])
        ir.Function(self.module, fopen_type, name="fopen")

        fclose_type = ir.FunctionType(i32, [file_ptr])
        ir.Function(self.module, fclose_type, name="fclose")

        fgets_type = ir.FunctionType(void_ptr, [void_ptr, i32, file_ptr])
        ir.Function(self.module, fgets_type, name="fgets")

        fputs_type = ir.FunctionType(i32, [void_ptr, file_ptr])
        ir.Function(self.module, fputs_type, name="fputs")

    def _get_or_declare_memcpy(self):
        """Get or declare llvm.memcpy intrinsic."""
        try:
            return self.module.get_global("llvm.memcpy.p0i8.p0i8.i32")
        except KeyError:
            # Declare llvm.memcpy.p0i8.p0i8.i32(i8* dst, i8* src, i32 len, i1 isvolatile)
            memcpy_type = ir.FunctionType(
                ir.VoidType(),
                [
                    ir.IntType(8).as_pointer(),
                    ir.IntType(8).as_pointer(),
                    ir.IntType(32),
                    ir.IntType(1),
                ],
            )
            return ir.Function(
                self.module, memcpy_type, name="llvm.memcpy.p0i8.p0i8.i32"
            )

    # --- Type Helper ---

    def _match_types(self, val, target_type, node=None):
        """
        Ensures 'val' matches 'target_type' by emitting LLVM conversion instructions.
        This handles the Pointer <-> Integer assignments that C allows.
        """
        if val is None:
            return None

        if isinstance(val, list):
            if not val:
                return None
            val_type = val[0].type
        else:
            val_type = val.type

        if isinstance(val_type, ir.VoidType) or isinstance(target_type, ir.VoidType):
            return None

        if val_type == target_type:
            return val

        comments = getattr(node, "comments", None) if node else None
        in_function = self.builder is not None

        if not in_function:
            if isinstance(val, ir.Constant):
                if isinstance(val_type, ir.IntType) and isinstance(
                    target_type, (ir.FloatType, ir.DoubleType)
                ):
                    return ir.Constant(target_type, float(val.constant))
                if isinstance(val_type, (ir.FloatType, ir.DoubleType)) and isinstance(
                    target_type, ir.IntType
                ):
                    return ir.Constant(target_type, int(val.constant))
                if isinstance(val_type, ir.IntType) and isinstance(
                    target_type, ir.IntType
                ):
                    return ir.Constant(target_type, int(val.constant))
                if isinstance(val_type, ir.PointerType) and isinstance(
                    target_type, ir.PointerType
                ):
                    return ir.Constant.bitcast(val, target_type)
            return None

        # 1. Integer to Pointer (e.g., int* p = 0;)
        if isinstance(target_type, ir.PointerType) and isinstance(val_type, ir.IntType):
            return self._add_comments(comments, self.builder.inttoptr(val, target_type))

        # 2. Pointer to Integer (e.g., int x = p;)
        if isinstance(target_type, ir.IntType) and isinstance(val_type, ir.PointerType):
            return self._add_comments(comments, self.builder.ptrtoint(val, target_type))

        # 3. Pointer to Pointer (e.g., char* c = (char*)int_ptr;)
        if isinstance(target_type, ir.PointerType) and isinstance(
            val.type, ir.PointerType
        ):
            if isinstance(getattr(val.type, "pointee", None), ir.ArrayType):
                zero = ir.Constant(ir.IntType(32), 0)
                decayed_ptr = self.builder.gep(val, [zero, zero])
                return self._add_comments(
                    comments, self.builder.bitcast(decayed_ptr, target_type)
                )
            return self._add_comments(comments, self.builder.bitcast(val, target_type))

        # 4. Numeric Conversions (Int <-> Float, sizing)
        if isinstance(val_type, ir.IntType) and isinstance(target_type, ir.IntType):
            if getattr(val_type, "width", 0) == getattr(target_type, "width", 0):
                return val
            if getattr(val_type, "width", 0) < getattr(target_type, "width", 0):
                return self._add_comments(comments, self.builder.sext(val, target_type))
            return self._add_comments(comments, self.builder.trunc(val, target_type))

        if isinstance(val_type, ir.IntType) and isinstance(
            target_type, (ir.FloatType, ir.DoubleType)
        ):
            return self._add_comments(comments, self.builder.sitofp(val, target_type))

        if isinstance(val_type, (ir.FloatType, ir.DoubleType)) and isinstance(
            target_type, ir.IntType
        ):
            if getattr(target_type, "width", 32) < 32:
                inter_int = self.builder.fptosi(val, ir.IntType(32), name="fp_to_i32")
                return self._add_comments(
                    comments,
                    self.builder.trunc(inter_int, target_type, name="i32_to_tiny"),
                )
            else:
                return self._add_comments(
                    comments, self.builder.fptosi(val, target_type)
                )

        return val

    def _get_zero_constant(self, llvm_type):
        """Returns a zero-initialized ir.Constant for the given LLVM type."""
        if isinstance(llvm_type, ir.ArrayType):
            elements = [
                self._get_zero_constant(llvm_type.element)
                for _ in range(llvm_type.count)
            ]
            return ir.Constant(llvm_type, elements)
        elif isinstance(llvm_type, (ir.FloatType, ir.DoubleType)):
            return ir.Constant(llvm_type, 0.0)
        elif isinstance(llvm_type, (ir.IdentifiedStructType, ir.LiteralStructType)):
            return ir.Constant(llvm_type, None)
        elif isinstance(llvm_type, ir.PointerType):
            return ir.Constant(llvm_type, None)  # Pointers need None, not 0
        else:
            return ir.Constant(llvm_type, 0)

    def _store_array_elements(
        self, base_ptr, init_list, llvm_type, current_indices=None, node=None
    ):
        if current_indices is None:
            if isinstance(getattr(base_ptr.type, "pointee", None), ir.ArrayType):
                current_indices = [ir.Constant(ir.IntType(32), 0)]
            else:
                current_indices = []

        element_type = getattr(
            llvm_type, "element", getattr(llvm_type, "pointee", llvm_type)
        )
        comments = getattr(node, "comments", None) if node else None

        for i, val in enumerate(init_list):
            idx_const = ir.Constant(ir.IntType(32), i)
            indices = current_indices + [idx_const]

            if isinstance(val, list):
                self._store_array_elements(base_ptr, val, element_type, indices, node)
            elif isinstance(element_type, ir.ArrayType):
                # Wrap the scalar in a list to initialize the first element of the sub-array
                self._store_array_elements(base_ptr, [val], element_type, indices, node)
            else:
                try:
                    elem_ptr = self.builder.gep(base_ptr, indices)
                    self._add_comments(comments, elem_ptr)
                    target_type = elem_ptr.type.pointee

                    actual_val = val
                    if isinstance(
                        getattr(actual_val, "type", None), ir.PointerType
                    ) and isinstance(
                        actual_val.type.pointee,
                        (ir.IdentifiedStructType, ir.LiteralStructType),
                    ):
                        if actual_val.type.pointee == target_type:
                            actual_val = self.builder.load(actual_val)

                    val_matched = self._match_types(actual_val, target_type, node)
                    if val_matched is not None:
                        inst = self.builder.store(val_matched, elem_ptr)
                        self._add_comments(comments, inst)
                except Exception:
                    val_matched = self._match_types(val, base_ptr.type.pointee, node)
                    if val_matched is not None:
                        try:
                            inst = self.builder.store(val_matched, base_ptr)
                            self._add_comments(comments, inst)
                        except Exception:
                            pass

    def _build_array_constant(self, init_list, llvm_type, node=None):
        """Recursively builds an ir.Constant for array initialization, padding missing elements."""
        if not isinstance(llvm_type, ir.ArrayType):
            return self._match_types(init_list, llvm_type, node)

        element_type = llvm_type.element
        expected_length = llvm_type.count

        elements = []
        for i in range(expected_length):
            if i < len(init_list):
                val = init_list[i]
                if isinstance(val, list):
                    elements.append(self._build_array_constant(val, element_type, node))
                elif isinstance(element_type, ir.ArrayType):
                    # Wrap the scalar in a list to initialize the first element of the sub-array
                    elements.append(
                        self._build_array_constant([val], element_type, node)
                    )
                else:
                    elements.append(self._match_types(val, element_type, node))
            else:
                elements.append(self._get_zero_constant(element_type))

        return ir.Constant(llvm_type, elements)

    def _add_comments(self, comments, instruction, c_line=None):
        if comments is None:
            return instruction
        if c_line:
            comments.append(f"{c_line}")
        instruction._inline_comment = comments[:]
        comments.clear()
        return instruction

    def _normalize_index(self, idx, node=None):
        if idx is None:
            return None

        comments = getattr(node, "comments", None) if node else None
        if isinstance(idx.type, (ir.FloatType, ir.DoubleType)):
            return self._add_comments(
                comments, self.builder.fptosi(idx, ir.IntType(32))
            )

        if isinstance(idx.type, ir.PointerType):
            as_int = self._add_comments(
                comments, self.builder.ptrtoint(idx, ir.IntType(64))
            )
            return self._add_comments(
                comments, self.builder.trunc(as_int, ir.IntType(32))
            )

        if isinstance(idx.type, ir.IntType):
            if idx.type.width < 32:
                return self._add_comments(
                    comments, self.builder.sext(idx, ir.IntType(32))
                )
            if idx.type.width > 32:
                return self._add_comments(
                    comments, self.builder.trunc(idx, ir.IntType(32))
                )
            return idx

        return None

    # --- Visit Methods ---

    def visit_ProgramNode(self, node: ProgramNode):
        for i in node.header_elements:
            yield i

    def visit_MainFunctionNode(self, node):
        func_type = ir.FunctionType(ir.IntType(32), [])
        func = ir.Function(self.module, func_type, name="main")
        block = func.append_basic_block(name="entry")
        self.builder = ir.IRBuilder(block)
        self.local_scopes = []
        self.struct_scopes = self.struct_scopes[:1]
        self.typedef_scopes = self.typedef_scopes[:1]
        self._enter_local_scope()

        noop = self.builder.add(
            ir.Constant(ir.IntType(32), 0), ir.Constant(ir.IntType(32), 0)
        )
        self._add_comments(getattr(node, "comments", None), noop, c_line=node)

        for stmt in node.statements:
            if self.builder.block.is_terminated:
                break
            yield stmt

        if not self.builder.block.is_terminated:
            ret_inst = self.builder.ret(ir.Constant(ir.IntType(32), 0))
            self._add_comments(getattr(node, "comments", None), ret_inst)

        self._exit_local_scope()

    def visit_IfNode(self, node):
        condition_val = yield node.condition

        zero = self._get_zero_constant(condition_val.type)
        cond_bool = self.builder.icmp_signed("!=", condition_val, zero, name="ifcond")

        func = self.builder.block.function
        true_block = func.append_basic_block(name="if.then")

        has_else = node.else_block is not None
        if has_else:
            false_block = func.append_basic_block(name="if.else")

        merge_block = func.append_basic_block(name="if.end")

        if has_else:
            self.builder.cbranch(cond_bool, true_block, false_block)
        else:
            self.builder.cbranch(cond_bool, true_block, merge_block)

        self.builder.position_at_end(true_block)
        if node.if_block:
            yield node.if_block

        if not self.builder.block.is_terminated:
            self.builder.branch(merge_block)

        if has_else:
            self.builder.position_at_end(false_block)
            yield node.else_block

            if not self.builder.block.is_terminated:
                self.builder.branch(merge_block)

        self.builder.position_at_end(merge_block)

    def visit_BlockNode(self, node):
        self._enter_local_scope()

        for stmt in node.statements:
            if self.builder.block.is_terminated:
                break
            yield stmt
        self._exit_local_scope()

    def visit_DeclarationNode(self, node):
        node.datatype = self._resolve_typedef_type(node.datatype)

        # Evaluate the initializer first so we know how many elements there are
        init_val = None
        if node.initializer:
            init_val = yield node.initializer

        # If it's an array and the size was omitted (e.g., int numbers[]), infer it!
        if getattr(node.datatype, "array_dimensions", None) and isinstance(
            init_val, list
        ):
            for i, dim in enumerate(node.datatype.array_dimensions):
                if dim is None:
                    # Create a LiteralNode to hold the inferred size
                    node.datatype.array_dimensions[i] = LiteralNode(
                        node.line,
                        node.column,
                        len(init_val),
                        TypeNode(node.line, node.column, BaseType.INT),
                    )

        # calculate the LLVM type
        llvm_type = self.type_map.get_llvm_type(node.datatype)

        if self.builder is None:
            ptr = ir.GlobalVariable(self.module, llvm_type, name=node.name)

            if init_val is not None:
                if (
                    isinstance(llvm_type, ir.ArrayType)
                    and llvm_type.element == ir.IntType(8)
                    and getattr(node.initializer, "value", None) is not None
                    and isinstance(node.initializer.value, str)
                ):
                    str_val = node.initializer.value + "\0"
                    bytes_val = bytearray(str_val.encode("utf-8"))

                    # Pad or truncate the string to match the array size
                    if len(bytes_val) < llvm_type.count:
                        bytes_val.extend(b"\0" * (llvm_type.count - len(bytes_val)))
                    else:
                        bytes_val = bytes_val[: llvm_type.count]

                    ptr.initializer = ir.Constant(llvm_type, bytes_val)

                elif isinstance(init_val, list):
                    ptr.initializer = self._build_array_constant(
                        init_val, llvm_type, node
                    )
                else:
                    matched = self._match_types(init_val, llvm_type, node)
                    if (
                        matched is not None
                        and getattr(matched, "type", None) == llvm_type
                    ):
                        ptr.initializer = matched
                    else:
                        ptr.initializer = self._get_zero_constant(llvm_type)
            else:
                # Uninitialized globals default to 0 in C
                ptr.initializer = self._get_zero_constant(llvm_type)

            self._define_symbol(node.name, ptr, is_global=True)
            return  # Stop here, we don't need builder instructions for globals

        # Local Scope (Inside a function)
        # Determine if this is a VLA by checking the AST dimensions directly
        is_vla = False
        if getattr(node.datatype, "array_dimensions", None):
            for dim in node.datatype.array_dimensions:
                if dim is not None and not hasattr(dim, "value"):
                    is_vla = True
                    break

        if is_vla:
            # Find and evaluate the dynamic dimension expression
            dynamic_size_val = None
            for dim in node.datatype.array_dimensions:
                if dim is not None and not hasattr(dim, "value"):
                    dynamic_size_val = yield dim
                    break

            # Safely normalize the dynamic size metric to an i32
            dynamic_size_val = self._normalize_index(dynamic_size_val, node)

            # Safely get the base element type by stripping array constraints
            datatype_copy = deepcopy(node.datatype)
            datatype_copy.array_dimensions = None
            element_type = self.type_map.get_llvm_type(datatype_copy)

            # Allocate the array with dynamic size directly on the stack frame
            alloca_ptr = self.builder.alloca(
                element_type, dynamic_size_val, name=node.name
            )
            # Mark it as a VLA so visit_VariableNode knows not to load from it
            ptr = VLAMarker(alloca_ptr)
        else:
            ptr = self.builder.alloca(llvm_type, name=node.name)

        if init_val is not None:
            if isinstance(init_val, list):
                zero_const = self._get_zero_constant(llvm_type)
                instruction = self.builder.store(zero_const, ptr)
                self._add_comments(getattr(node, "comments", None), instruction)
                self._store_array_elements(ptr, init_val, llvm_type, node=node)
            else:
                actual_val = init_val
                if isinstance(
                    getattr(actual_val, "type", None), ir.PointerType
                ) and isinstance(
                    actual_val.type.pointee,
                    (ir.IdentifiedStructType, ir.LiteralStructType),
                ):
                    if actual_val.type.pointee == llvm_type:
                        actual_val = self.builder.load(actual_val)

                # Special case: initializing a char array with a string pointer
                if (
                    isinstance(llvm_type, ir.ArrayType)
                    and llvm_type.element == ir.IntType(8)
                    and isinstance(getattr(actual_val, "type", None), ir.PointerType)
                    and actual_val.type.pointee == ir.IntType(8)
                ):
                    # Copy string to array using memcpy
                    memcpy_fn = self._get_or_declare_memcpy()
                    array_as_ptr = self.builder.bitcast(ptr, ir.IntType(8).as_pointer())
                    array_size = ir.Constant(ir.IntType(32), llvm_type.count)
                    is_volatile = ir.Constant(ir.IntType(1), 0)
                    self.builder.call(
                        memcpy_fn, [array_as_ptr, actual_val, array_size, is_volatile]
                    )
                else:
                    init_val = self._match_types(actual_val, llvm_type, node)
                    if (
                        init_val is not None
                        and getattr(init_val, "type", None) == llvm_type
                    ):
                        instruction = self.builder.store(init_val, ptr)
                        self._add_comments(
                            getattr(node, "comments", None), instruction, node
                        )

        self._define_symbol(node.name, ptr)

    def visit_AssignmentNode(self, node):
        val = yield node.expression
        if val is None:
            val = ir.Constant(ir.IntType(32), 0)

        ptr = yield from self._get_lvalue_ptr(node.target, node)
        if (
            ptr is not None
            and getattr(ptr, "type", None) is not None
            and isinstance(ptr.type, ir.PointerType)
        ):
            if isinstance(val.type, ir.PointerType) and isinstance(
                val.type.pointee, ir.IdentifiedStructType
            ):
                if val.type.pointee == ptr.type.pointee:
                    val = self.builder.load(val)
            val = self._match_types(val, ptr.type.pointee, node)
        comments = getattr(node, "comments", None)

        if (
            ptr is not None
            and getattr(ptr, "type", None) is not None
            and isinstance(ptr.type, ir.PointerType)
        ):
            val = self._match_types(val, ptr.type.pointee, node)
            if val is not None and getattr(val, "type", None) == ptr.type.pointee:
                store_inst = self.builder.store(val, ptr)
                self._add_comments(comments, store_inst, node)

        return val

    def _get_lvalue_ptr(self, target, node=None):
        comments = getattr(node or target, "comments", None)

        if isinstance(target, VariableNode):
            return self._lookup_symbol(target.name)

        if isinstance(target, UnaryOpNode) and target.operator == "*":
            ptr = yield target.operand
            while getattr(ptr, "type", None) is not None and isinstance(
                getattr(ptr.type, "pointee", None), ir.ArrayType
            ):
                zero = ir.Constant(ir.IntType(32), 0)
                ptr = self.builder.gep(ptr, [zero, zero])
            return ptr

        if isinstance(target, ArrayAccessNode):
            arr_ptr = yield target.array
            idx = yield target.index
            if arr_ptr is None or idx is None:
                return None
            idx = self._normalize_index(idx, target)
            if idx is None:
                return None

            if not isinstance(arr_ptr.type, ir.PointerType):
                arr_ptr = self.builder.inttoptr(arr_ptr, ir.PointerType(ir.IntType(32)))
                self._add_comments(comments, arr_ptr)

            pointee = arr_ptr.type.pointee
            if isinstance(pointee, ir.ArrayType):
                zero = ir.Constant(ir.IntType(32), 0)
                gep = self.builder.gep(arr_ptr, [zero, idx])
            else:
                gep = self.builder.gep(arr_ptr, [idx])
            return self._add_comments(comments, gep)

        if isinstance(target, MemberAccessNode):
            return (yield from self._get_member_ptr(target))

        return None

    def _get_member_ptr(self, node):
        comments = getattr(node, "comments", None)
        if node.pointer:
            base_ptr = yield node.object
        else:
            obj = node.object
            if isinstance(obj, VariableNode):
                base_ptr = self._lookup_symbol(obj.name)
            elif isinstance(obj, UnaryOpNode) and obj.operator == "*":
                base_ptr = yield obj.operand
            elif isinstance(obj, ArrayAccessNode):
                base_ptr = yield from self._get_lvalue_ptr(obj, node)
            elif isinstance(obj, MemberAccessNode):
                base_ptr = yield from self._get_member_ptr(obj)
            else:
                base_ptr = None

        if base_ptr is None or not isinstance(base_ptr.type, ir.PointerType):
            return None

        struct_ty = base_ptr.type.pointee
        struct_info = self.struct_info_by_type_id.get(id(struct_ty))
        if struct_info is None:
            obj_type = getattr(node.object, "inferred_type", None)
            struct_name = getattr(obj_type, "struct_name", None) if obj_type else None
            if struct_name:
                struct_info = self._lookup_struct_info(struct_name)

        if struct_info is None:
            return None

        if node.field not in struct_info["field_indices"]:
            return None

        if struct_info.get("is_union", False):
            field_ast_type = struct_info["field_types"][node.field]
            llvm_field_type = self.type_map.get_llvm_type(field_ast_type)

            cast_ptr = self.builder.bitcast(base_ptr, llvm_field_type.as_pointer())
            return self._add_comments(comments, cast_ptr)

        else:
            field_idx = struct_info["field_indices"][node.field]
            zero = ir.Constant(ir.IntType(32), 0)
            idx = ir.Constant(ir.IntType(32), field_idx)

            gep = self.builder.gep(base_ptr, [zero, idx])
            return self._add_comments(comments, gep)

    def visit_CastNode(self, node):
        node.target_type = self._resolve_typedef_type(node.target_type)
        val = yield node.expression
        if val is None:
            return None
        target_ll_type = self.type_map.get_llvm_type(node.target_type)
        return self._match_types(val, target_ll_type, node)

    def visit_BinaryOpNode(self, node):
        comments = getattr(node, "comments", None)

        if node.operator in ["&&", "||"]:
            left = yield node.left
            if left is None:
                return None

            if isinstance(
                getattr(getattr(left, "type", None), "pointee", None), ir.ArrayType
            ):
                zero = ir.Constant(ir.IntType(32), 0)
                left = self.builder.gep(left, [zero, zero])

            func = self.builder.block.function
            eval_right_block = func.append_basic_block(name="logic.right")
            merge_block = func.append_basic_block(name="logic.end")

            is_float = isinstance(left.type, (ir.FloatType, ir.DoubleType))
            zero_l = self._get_zero_constant(left.type)
            l_bool = (
                self.builder.fcmp_ordered("!=", left, zero_l)
                if is_float
                else self.builder.icmp_signed("!=", left, zero_l)
            )

            if node.operator == "&&":
                self.builder.cbranch(l_bool, eval_right_block, merge_block)
            else:
                self.builder.cbranch(l_bool, merge_block, eval_right_block)

            left_block = self.builder.block

            self.builder.position_at_end(eval_right_block)
            right = yield node.right

            is_float_r = isinstance(right.type, (ir.FloatType, ir.DoubleType))
            zero_r = self._get_zero_constant(right.type)
            r_bool = (
                self.builder.fcmp_ordered("!=", right, zero_r)
                if is_float_r
                else self.builder.icmp_signed("!=", right, zero_r)
            )
            right_block = self.builder.block
            self.builder.branch(merge_block)

            self.builder.position_at_end(merge_block)
            phi = self.builder.phi(ir.IntType(1))

            if node.operator == "&&":
                phi.add_incoming(ir.Constant(ir.IntType(1), 0), left_block)
                phi.add_incoming(r_bool, right_block)
            else:
                phi.add_incoming(ir.Constant(ir.IntType(1), 1), left_block)
                phi.add_incoming(r_bool, right_block)

            return self._add_comments(
                comments, self.builder.zext(phi, ir.IntType(32)), node
            )

        right = yield node.right
        left = yield node.left

        if left is None or right is None:
            return None

        if isinstance(
            getattr(getattr(left, "type", None), "pointee", None), ir.ArrayType
        ):
            zero = ir.Constant(ir.IntType(32), 0)
            left = self.builder.gep(left, [zero, zero])

        if isinstance(
            getattr(getattr(right, "type", None), "pointee", None), ir.ArrayType
        ):
            zero = ir.Constant(ir.IntType(32), 0)
            right = self.builder.gep(right, [zero, zero])

        is_left_ptr = isinstance(left.type, ir.PointerType)
        is_right_ptr = isinstance(right.type, ir.PointerType)
        comments = getattr(node, "comments", None)

        # Handle pointer arithmetic
        if node.operator == "+":
            if is_left_ptr:
                right = self._normalize_index(right, node)
                if right is None:
                    return None
                return self._add_comments(
                    comments, self.builder.gep(left, [right]), node
                )
            if is_right_ptr:
                left = self._normalize_index(left, node)
                if left is None:
                    return None
                return self._add_comments(
                    comments, self.builder.gep(right, [left]), node
                )

        if node.operator == "-":
            if is_left_ptr and not is_right_ptr:
                right = self._normalize_index(right, node)
                if right is None:
                    return None
                neg_right = self._add_comments(comments, self.builder.neg(right))
                return self._add_comments(
                    comments, self.builder.gep(left, [neg_right]), node
                )
            if is_left_ptr and is_right_ptr:
                l_int = self._add_comments(
                    comments, self.builder.ptrtoint(left, ir.IntType(64))
                )
                r_int = self._add_comments(
                    comments, self.builder.ptrtoint(right, ir.IntType(64))
                )
                diff = self._add_comments(comments, self.builder.sub(l_int, r_int))

                null_ptr = ir.Constant(left.type, None)
                gep_one = self.builder.gep(null_ptr, [ir.Constant(ir.IntType(32), 1)])
                size_int = self.builder.ptrtoint(gep_one, ir.IntType(64))

                diff_divided = self.builder.sdiv(diff, size_int)

                return self._add_comments(
                    comments, self.builder.trunc(diff_divided, ir.IntType(32)), node
                )

        # Standard arithmetic (non-pointer)
        if is_left_ptr:
            left = self._add_comments(
                comments, self.builder.ptrtoint(left, ir.IntType(64))
            )
        if is_right_ptr:
            right = self._add_comments(
                comments, self.builder.ptrtoint(right, ir.IntType(64))
            )

        if left.type != right.type:
            if isinstance(left.type, (ir.FloatType, ir.DoubleType)) and not isinstance(
                right.type, (ir.FloatType, ir.DoubleType)
            ):
                right = self._match_types(right, left.type, node)
            elif isinstance(
                right.type, (ir.FloatType, ir.DoubleType)
            ) and not isinstance(left.type, (ir.FloatType, ir.DoubleType)):
                left = self._match_types(left, right.type, node)
            elif getattr(left.type, "width", 0) > getattr(right.type, "width", 0):
                right = self._match_types(right, left.type, node)
            else:
                left = self._match_types(left, right.type, node)

        is_float = isinstance(left.type, (ir.FloatType, ir.DoubleType))

        # Arithmetics
        if node.operator == "+":
            inst = (
                self.builder.fadd(left, right)
                if is_float
                else self.builder.add(left, right)
            )
            return self._add_comments(comments, inst, node)
        if node.operator == "-":
            inst = (
                self.builder.fsub(left, right)
                if is_float
                else self.builder.sub(left, right)
            )
            return self._add_comments(comments, inst, node)
        if node.operator == "*":
            inst = (
                self.builder.fmul(left, right)
                if is_float
                else self.builder.mul(left, right)
            )
            return self._add_comments(comments, inst, node)
        if node.operator == "/":
            inst = (
                self.builder.fdiv(left, right)
                if is_float
                else self.builder.sdiv(left, right)
            )
            return self._add_comments(comments, inst, node)
        if node.operator == "%":
            inst = (
                self.builder.frem(left, right)
                if is_float
                else self.builder.srem(left, right)
            )
            return self._add_comments(comments, inst, node)

        # Bitwise
        if node.operator == "<<":
            return self._add_comments(comments, self.builder.shl(left, right), node)
        if node.operator == ">>":
            return self._add_comments(comments, self.builder.ashr(left, right), node)
        if node.operator == "&":
            return self._add_comments(comments, self.builder.and_(left, right), node)
        if node.operator == "|":
            return self._add_comments(comments, self.builder.or_(left, right), node)
        if node.operator == "^":
            return self._add_comments(comments, self.builder.xor(left, right), node)

        # Logical AND / OR (returns i32)
        if node.operator in ["&&", "||"]:
            zero_l = self._get_zero_constant(left.type)
            zero_r = self._get_zero_constant(right.type)

            l_bool = (
                self.builder.fcmp_ordered("!=", left, zero_l)
                if is_float
                else self.builder.icmp_signed("!=", left, zero_l)
            )
            self._add_comments(comments, l_bool)

            r_bool = (
                self.builder.fcmp_ordered("!=", right, zero_r)
                if is_float
                else self.builder.icmp_signed("!=", right, zero_r)
            )
            self._add_comments(comments, r_bool)

            res_bool = (
                self.builder.and_(l_bool, r_bool)
                if node.operator == "&&"
                else self.builder.or_(l_bool, r_bool)
            )
            self._add_comments(comments, res_bool)
            return self._add_comments(
                comments, self.builder.zext(res_bool, ir.IntType(32)), node
            )

        # Comparisons (returns i32)
        if node.operator in ["==", "!=", "<", "<=", ">", ">="]:
            if is_float:
                res = self.builder.fcmp_ordered(node.operator, left, right)
            else:
                res = self.builder.icmp_signed(node.operator, left, right)
            self._add_comments(comments, res)
            return self._add_comments(
                comments, self.builder.zext(res, ir.IntType(32)), node
            )

        return None

    def visit_VariableNode(self, node):
        if hasattr(node, "enum_value"):
            return ir.Constant(ir.IntType(32), int(node.enum_value))

        ptr = self._lookup_symbol(node.name)
        if ptr is None:
            return None

        # Handle VLA markers - return the pointer directly without loading
        if isinstance(ptr, VLAMarker):
            return ptr.ptr

        pointee_type = ptr.type.pointee
        if isinstance(pointee_type, (ir.ArrayType, ir.IdentifiedStructType)):
            return ptr
        else:
            inst = self.builder.load(ptr, name=node.name)
            return self._add_comments(getattr(node, "comments", None), inst, node)

    def visit_LiteralNode(self, node):
        llvm_type = self.type_map.get_llvm_type(node.datatype)
        is_string_literal = False
        if getattr(node.datatype, "base_type", None) == BaseType.CHAR:
            if getattr(node.datatype, "ptr_depth", 0) > 0:
                is_string_literal = True
            elif getattr(node.datatype, "array_dimensions", None):
                is_string_literal = True

        if is_string_literal and isinstance(node.value, str):
            str_val = node.value + "\0"
            const_str = ir.Constant(
                ir.ArrayType(ir.IntType(8), len(str_val)),
                bytearray(str_val.encode("utf-8")),
            )
            g_var = ir.GlobalVariable(
                self.module, const_str.type, name=f".str{id(node)}"
            )
            g_var.initializer = const_str
            g_var.global_constant = True

            if self.builder is None:
                inst = ir.Constant.bitcast(g_var, ir.IntType(8).as_pointer())
            else:
                inst = self.builder.bitcast(g_var, ir.IntType(8).as_pointer())
            return self._add_comments(getattr(node, "comments", None), inst, node)

        # Safely convert literal values to match the llvm_type to prevent clang errors
        val = node.value
        if isinstance(llvm_type, ir.IntType):
            try:
                val = int(val)
            except ValueError:
                try:
                    val = int(float(val))
                except Exception:
                    val = 0
            except Exception:
                val = 0
        elif isinstance(llvm_type, (ir.FloatType, ir.DoubleType)):
            try:
                val = float(val)
            except Exception:
                val = 0.0

        return ir.Constant(llvm_type, val)

    def visit_UnaryOpNode(self, node):
        comments = getattr(node, "comments", None)
        if node.operator == "&":
            if isinstance(node.operand, VariableNode):
                return self._lookup_symbol(node.operand.name)
            elif isinstance(node.operand, ArrayAccessNode):
                arr_ptr = yield node.operand.array
                idx = yield node.operand.index
                if arr_ptr is None or idx is None:
                    return None

                idx = self._normalize_index(idx, node)
                if idx is None:
                    return None

                if not isinstance(arr_ptr.type, ir.PointerType):
                    arr_ptr = self.builder.inttoptr(
                        arr_ptr, ir.PointerType(ir.IntType(32))
                    )
                    self._add_comments(comments, arr_ptr)

                pointee = arr_ptr.type.pointee

                if isinstance(pointee, ir.ArrayType):
                    zero = ir.Constant(ir.IntType(32), 0)
                    return self._add_comments(
                        comments, self.builder.gep(arr_ptr, [zero, idx]), node
                    )
                return self._add_comments(
                    comments, self.builder.gep(arr_ptr, [idx]), node
                )
            elif isinstance(node.operand, UnaryOpNode) and node.operand.operator == "*":
                return (yield node.operand.operand)
            elif isinstance(node.operand, MemberAccessNode):
                return (yield from self._get_member_ptr(node.operand))

        if node.operator in ("++", "--"):
            ptr = None
            ptr = yield from self._get_lvalue_ptr(node.operand, node)

            if ptr is not None:
                old_val = self.builder.load(ptr)
                self._add_comments(comments, old_val)

                if isinstance(old_val.type, ir.PointerType):
                    step = ir.Constant(
                        ir.IntType(32), 1 if node.operator == "++" else -1
                    )
                    new_val = self.builder.gep(old_val, [step])
                else:
                    is_float = isinstance(old_val.type, (ir.FloatType, ir.DoubleType))
                    one = ir.Constant(old_val.type, 1.0 if is_float else 1)

                    if node.operator == "++":
                        new_val = (
                            self.builder.fadd(old_val, one)
                            if is_float
                            else self.builder.add(old_val, one)
                        )
                    else:
                        new_val = (
                            self.builder.fsub(old_val, one)
                            if is_float
                            else self.builder.sub(old_val, one)
                        )
                self._add_comments(comments, new_val)

                store_inst = self.builder.store(new_val, ptr)
                self._add_comments(comments, store_inst, node)

                return old_val if getattr(node, "postfix", False) else new_val
            return None

        val = yield node.operand
        if val is None:
            return None

        if node.operator == "*":
            if not isinstance(val.type, ir.PointerType):
                val = self.builder.inttoptr(val, ir.PointerType(ir.IntType(32)))
                self._add_comments(comments, val)

                # 1. Decay the operand if it points to an array
            if isinstance(getattr(val.type, "pointee", None), ir.ArrayType):
                zero = ir.Constant(ir.IntType(32), 0)
                val = self.builder.gep(val, [zero, zero])

                # 2. Check the dereferenced result. If it's an array, it decays back to a pointer (no load!)
            if isinstance(getattr(val.type, "pointee", None), ir.ArrayType):
                zero = ir.Constant(ir.IntType(32), 0)
                val = self.builder.gep(val, [zero, zero])
                return self._add_comments(comments, val, node)

                # 3. Standard primitive/pointer load
            return self._add_comments(comments, self.builder.load(val), node)
        if node.operator == "-":
            inst = (
                self.builder.fneg(val)
                if isinstance(val.type, ir.FloatType)
                else self.builder.neg(val)
            )
            return self._add_comments(comments, inst, node)
        if node.operator == "!":
            if isinstance(val.type, (ir.FloatType, ir.DoubleType)):
                cmp = self.builder.fcmp_ordered(
                    "==", val, self._get_zero_constant(val.type)
                )
            else:
                if isinstance(val.type, ir.PointerType):
                    val = self.builder.ptrtoint(val, ir.IntType(64))
                    self._add_comments(comments, val)
                cmp = self.builder.icmp_signed(
                    "==", val, self._get_zero_constant(val.type)
                )
            self._add_comments(comments, cmp)
            return self._add_comments(
                comments, self.builder.zext(cmp, ir.IntType(32)), node
            )
        if node.operator == "~":
            return self._add_comments(comments, self.builder.not_(val), node)
        return val

    def visit_ArrayAccessNode(self, node):
        arr_ptr = yield node.array
        idx = yield node.index

        if arr_ptr is None or idx is None:
            return None

        comments = getattr(node, "comments", None)

        if isinstance(idx.type, (ir.FloatType, ir.DoubleType)):
            idx = self._add_comments(comments, self.builder.fptosi(idx, ir.IntType(32)))
        elif isinstance(idx.type, ir.IntType) and idx.type.width != 32:
            if idx.type.width < 32:
                idx = self._add_comments(
                    comments, self.builder.sext(idx, ir.IntType(32))
                )
            else:
                idx = self._add_comments(
                    comments, self.builder.trunc(idx, ir.IntType(32))
                )

        if not isinstance(arr_ptr.type, ir.PointerType):
            arr_ptr = self._add_comments(
                comments, self.builder.inttoptr(arr_ptr, ir.PointerType(ir.IntType(32)))
            )

        pointee = arr_ptr.type.pointee
        if isinstance(pointee, ir.ArrayType):
            zero = ir.Constant(ir.IntType(32), 0)
            ptr = self._add_comments(comments, self.builder.gep(arr_ptr, [zero, idx]))
        else:
            ptr = self._add_comments(comments, self.builder.gep(arr_ptr, [idx]))

        if isinstance(ptr.type.pointee, ir.ArrayType):
            return self._add_comments(comments, ptr, node)
        else:
            return self._add_comments(comments, self.builder.load(ptr), node)

    def visit_MemberAccessNode(self, node):
        ptr = yield from self._get_member_ptr(node)
        if ptr is None:
            return None

        comments = getattr(node, "comments", None)
        if isinstance(ptr.type.pointee, ir.ArrayType):
            return self._add_comments(comments, ptr, node)
        return self._add_comments(comments, self.builder.load(ptr), node)

    def visit_InitializerListNode(self, node):
        elements = []
        for element in node.elements:
            elem_val = yield element
            elements.append(elem_val)
        return elements

    def visit_FunctionCallNode(self, node):
        if not hasattr(node.function, "name"):
            return None
        mangled_name = getattr(node, "mangled_name", node.function.name)
        if not mangled_name:
            mangled_name = node.function.name

        args = []
        for arg_node in node.arguments:
            arg_val = yield arg_node
            if arg_val is not None:
                args.append(arg_val)

        try:
            func = self.module.get_global(mangled_name)
        except KeyError:
            func = None

        comments = getattr(node, "comments", None)

        for i, arg_val in enumerate(args):
            if isinstance(arg_val.type, ir.PointerType):
                pointee = arg_val.type.pointee
                if isinstance(pointee, ir.ArrayType):
                    zero = ir.Constant(ir.IntType(32), 0)
                    args[i] = self._add_comments(
                        comments, self.builder.gep(arg_val, [zero, zero])
                    )

        if func is not None:
            f_type = (
                func.type.pointee
                if isinstance(func.type, ir.PointerType)
                else func.type
            )
            is_var_arg = getattr(f_type, "var_arg", False)
            num_fixed_args = len(f_type.args)
        else:
            is_var_arg = True
            num_fixed_args = 0

        if is_var_arg:
            for i in range(num_fixed_args, len(args)):
                arg_val = args[i]
                if isinstance(arg_val.type, ir.FloatType):
                    args[i] = self._add_comments(
                        comments, self.builder.fpext(arg_val, ir.DoubleType())
                    )
                elif isinstance(arg_val.type, ir.IntType) and arg_val.type.width < 32:
                    if arg_val.type.width == 1:
                        args[i] = self._add_comments(
                            comments, self.builder.zext(arg_val, ir.IntType(32))
                        )
                    else:
                        args[i] = self._add_comments(
                            comments, self.builder.sext(arg_val, ir.IntType(32))
                        )

        if func is None:
            # Fallback for implicitly declared functions (e.g., if printf wasn't declared in the module)
            ret_type = ir.IntType(32)
            arg_types = [arg.type for arg in args]
            f_type = ir.FunctionType(ret_type, arg_types, var_arg=True)
            func = ir.Function(self.module, f_type, name=mangled_name)

        # Ensure fixed arguments match the function signature
        if func is not None:
            if not is_var_arg and len(args) != num_fixed_args:
                return None
            if is_var_arg and len(args) < num_fixed_args:
                return None

            for i in range(num_fixed_args):
                expected_type = f_type.args[i]
                args[i] = self._match_types(args[i], expected_type, node)
                if args[i] is None:
                    return None

        # Emit the call instruction
        call_inst = self.builder.call(func, args)
        if isinstance(f_type.return_type, ir.VoidType):
            self._add_comments(comments, call_inst, node)
            return None
        return self._add_comments(comments, call_inst, node)

    def visit_FunctionNode(self, node):
        """Generates LLVM IR for user-defined functions."""
        node.return_type = self._resolve_typedef_type(node.return_type)
        for param in node.parameters:
            param.datatype = self._resolve_typedef_type(param.datatype)

        # Map return type
        ret_type = self.type_map.get_llvm_type(node.return_type)

        # Map parameter types
        param_types = []
        for param in node.parameters:
            param_types.append(self.type_map.get_llvm_type(param.datatype))

        # Create function type and declare it in the module
        func_type = ir.FunctionType(ret_type, param_types)
        mangled_name = getattr(node, "mangled_name", node.name)
        try:
            func = self.module.get_global(mangled_name)
        except KeyError:
            func = ir.Function(self.module, func_type, name=mangled_name)

        # Name the arguments
        for i, param in enumerate(node.parameters):
            func.args[i].name = param.name

        # If it's just a declaration (prototype), stop here
        if not node.is_definition:
            return

        # Already defined: don't emit a second function body.
        if getattr(func, "blocks", None):
            return

        # Setup entry block and builder
        block = func.append_basic_block(name="entry")
        old_builder = self.builder
        old_scopes = self.local_scopes
        old_struct_scopes = self.struct_scopes
        self.builder = ir.IRBuilder(block)
        self.local_scopes = []
        self.struct_scopes = self.struct_scopes[:1]
        self.typedef_scopes = self.typedef_scopes[:1]
        self._enter_local_scope()

        # Allocate memory for parameters so they act like local variables
        for i, param in enumerate(node.parameters):
            ptr = self.builder.alloca(func.args[i].type, name=param.name + "_addr")
            self.builder.store(func.args[i], ptr)
            self._define_symbol(param.name, ptr)

        # Visit function body
        for stmt in node.statements:
            if self.builder.block.is_terminated:
                break
            yield stmt

        # Ensure the block is terminated (handle missing returns)
        if not self.builder.block.is_terminated:
            if isinstance(ret_type, ir.VoidType):
                self.builder.ret_void()
            else:
                # Return a default 0 if the user forgot a return statement
                zero = self._get_zero_constant(ret_type)
                self.builder.ret(zero)

        # Restore previous builder (useful for nested or sequential generation)
        self.builder = old_builder
        self.local_scopes = old_scopes
        self.struct_scopes = old_struct_scopes

    def visit_TypedefNode(self, node):
        self._current_typedef_scope()[node.name] = self._resolve_typedef_type(
            node.target_type
        )

    def visit_StructNode(self, node):
        self._register_struct(node)

    def visit_ReturnNode(self, node):
        """Generates LLVM IR for return statements."""
        comments = getattr(node, "comments", None)

        if node.expression:
            retval = yield node.expression

            current_func = self.builder.block.function
            expected_ret_type = current_func.type.pointee.return_type

            if str(expected_ret_type) == "void":
                ret_inst = self.builder.ret_void()
            else:
                retval = self._match_types(retval, expected_ret_type, node)
                ret_inst = self.builder.ret(retval)
        else:
            ret_inst = self.builder.ret_void()

        self._add_comments(comments, ret_inst, node)

    def visit_WhileLoopNode(self, node):
        func = self.builder.block.function

        cond_block = func.append_basic_block(name="while.cond")
        body_block = func.append_basic_block(name="while.body")
        end_block = func.append_basic_block(name="while.end")

        self.builder.branch(cond_block)
        self.builder.position_at_end(cond_block)

        if node.condition:
            condition_val = yield node.condition

            is_float = isinstance(condition_val.type, (ir.FloatType, ir.DoubleType))

            if is_float:
                zero = self._get_zero_constant(condition_val.type)
                cond_bool = self.builder.fcmp_ordered(
                    "!=", condition_val, zero, name="whilecond"
                )
            else:
                if isinstance(condition_val.type, ir.PointerType):
                    condition_val = self.builder.ptrtoint(condition_val, ir.IntType(64))

                zero = self._get_zero_constant(condition_val.type)
                cond_bool = self.builder.icmp_signed(
                    "!=", condition_val, zero, name="whilecond"
                )

            self.builder.cbranch(cond_bool, body_block, end_block)
        else:
            self.builder.branch(body_block)

        self.builder.position_at_end(body_block)

        self.loop_blocks.append((cond_block, end_block))

        if node.body_block:
            yield node.body_block

        self.loop_blocks.pop()

        if not self.builder.block.is_terminated:
            self.builder.branch(cond_block)

        self.builder.position_at_end(end_block)

    def visit_BreakNode(self, node):
        if self.loop_blocks:
            _, end_block = self.loop_blocks[-1]
            if not self.builder.block.is_terminated:
                self.builder.branch(end_block)

    def visit_ContinueNode(self, node):
        if self.loop_blocks:
            cond_block, _ = self.loop_blocks[-1]
            if not self.builder.block.is_terminated:
                self.builder.branch(cond_block)

    def visit_SizeOfNode(self, node):
        if node.is_type:
            resolved_type = self._resolve_typedef_type(node.target)
            llvm_type = self.type_map.get_llvm_type(resolved_type)
        else:
            llvm_type = self.type_map.get_llvm_type(node.target.inferred_type)

        null_ptr = ir.Constant(llvm_type.as_pointer(), None)
        gep = self.builder.gep(null_ptr, [ir.Constant(ir.IntType(32), 1)])
        size_64 = self.builder.ptrtoint(gep, ir.IntType(64))
        size_32 = self.builder.trunc(size_64, ir.IntType(32))
        return self._add_comments(getattr(node, "comments", None), size_32, node)
