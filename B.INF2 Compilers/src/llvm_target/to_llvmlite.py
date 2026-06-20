from llvmlite import ir
from src.parser.AST.Node import BaseType, TypeNode


def _try_evaluate_constant(dim_node):
    """Try to evaluate a dimension node to a constant value.
    Returns the integer value if successful, None otherwise."""
    if dim_node is None:
        return None

    # If it's a LiteralNode, extract the value
    if hasattr(dim_node, "value"):
        try:
            return int(dim_node.value)
        except (ValueError, TypeError):
            return None

    # If it's a BinaryOpNode with two LiteralNode operands, evaluate it
    if (
        hasattr(dim_node, "operator")
        and hasattr(dim_node, "left")
        and hasattr(dim_node, "right")
    ):
        try:
            left_val = _try_evaluate_constant(dim_node.left)
            right_val = _try_evaluate_constant(dim_node.right)
            if left_val is not None and right_val is not None:
                if dim_node.operator == "+":
                    return left_val + right_val
                elif dim_node.operator == "-":
                    return left_val - right_val
                elif dim_node.operator == "*":
                    return left_val * right_val
                elif dim_node.operator == "/":
                    if right_val != 0:
                        return left_val // right_val
                elif dim_node.operator == "%":
                    if right_val != 0:
                        return left_val % right_val
        except (ValueError, TypeError, ZeroDivisionError):
            return None

    # If it's a UnaryOpNode with a LiteralNode operand, evaluate it
    if hasattr(dim_node, "operator") and hasattr(dim_node, "operand"):
        try:
            val = _try_evaluate_constant(dim_node.operand)
            if val is not None:
                if dim_node.operator == "-":
                    return -val
                elif dim_node.operator == "+":
                    return val
                elif dim_node.operator == "~":
                    return ~val
        except (ValueError, TypeError):
            return None

    return None


class LLVMTypeMap:
    def __init__(self, struct_resolver=None, typedef_resolver=None):
        self.struct_resolver = struct_resolver
        self.typedef_resolver = typedef_resolver
        self.base_map = {
            BaseType.INT: ir.IntType(32),
            BaseType.FLOAT: ir.FloatType(),
            BaseType.CHAR: ir.IntType(8),
            BaseType.VOID: ir.VoidType(),
        }

    def get_llvm_type(self, node: TypeNode):
        if self.typedef_resolver and getattr(node, "alias_name", None):
            resolved = self.typedef_resolver(node)
            if resolved is not None:
                node = resolved

        if node.base_type in (BaseType.STRUCT, BaseType.UNION):
            llvm_type = None
            if self.struct_resolver and getattr(node, "struct_name", None):
                llvm_type = self.struct_resolver(node.struct_name)
            if llvm_type is None:
                llvm_type = ir.IntType(8)
        else:
            llvm_type = self.base_map.get(node.base_type, ir.IntType(32))

        for _ in range(node.ptr_depth):
            llvm_type = llvm_type.as_pointer()

        # LLVM Arrays nesten van achteren naar voren!
        if hasattr(node, "array_dimensions") and node.array_dimensions:
            for dim_node in reversed(node.array_dimensions):
                if dim_node is None:
                    llvm_type = llvm_type.as_pointer()
                else:
                    # Try to evaluate the dimension as a constant
                    dim_val = None
                    if hasattr(dim_node, "value"):
                        dim_val = dim_node.value
                    else:
                        # Try to evaluate constant expressions like 1+1
                        dim_val = _try_evaluate_constant(dim_node)

                    if dim_val is not None:
                        llvm_type = ir.ArrayType(llvm_type, dim_val)
                    else:
                        # It's a truly dynamic dimension (depends on a variable)
                        llvm_type = llvm_type.as_pointer()

        return llvm_type
