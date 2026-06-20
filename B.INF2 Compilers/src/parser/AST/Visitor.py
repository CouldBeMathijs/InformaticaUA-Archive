import types


class AstVisitor:
    def visit(self, root_node):
        if root_node is None:
            return None

        stack = [self._create_generator(root_node)]
        last_result = None

        while stack:
            gen = stack[-1]
            try:
                child_to_visit = gen.send(last_result)
                stack.append(self._create_generator(child_to_visit))
                last_result = None
            except StopIteration as e:
                stack.pop()
                last_result = e.value

        return last_result

    def _create_generator(self, node):
        if node is None:

            def null_gen():
                return None
                yield

            return null_gen()

        method_name = f"visit_{type(node).__name__}"
        visitor_method = getattr(self, method_name, self.visit_default)
        result = visitor_method(node)

        if isinstance(result, types.GeneratorType):
            return result
        else:

            def wrap_value():
                return result
                yield

            return wrap_value()

    def visit_default(self, node):
        results = []
        if hasattr(node, "children"):
            for child in node.children:
                res = yield child
                results.append(res)
        return results
