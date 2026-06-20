from antlr4.error.ErrorListener import ErrorListener
from src.issue_printer import IssuePrinter, SyntaxErrorNode, missing_main_error


class CustomAntlrErrorListener(ErrorListener):
    def __init__(self, file_name="<stdin>"):
        super().__init__()
        self.file_name = file_name
        self.issues = []

    def syntaxError(self, recognizer, offendingSymbol, line, column, msg, e):
        node = SyntaxErrorNode(line, column)

        if "'#include', 'int'" in msg:
            error = missing_main_error(node)
            self.issues.append(error)
            self.issues.append(msg)
            return

        elif "expecting ';'" in msg and "=" in msg:
            friendly_msg = (
                "Cannot assign a value to an rvalue (check your assignment target)."
            )
            code = "E003"

        elif "expecting" in msg:
            expected = msg.split("expecting")[-1].strip()
            friendly_msg = (
                f"Syntax Error: Expected {expected} but found '{offendingSymbol.text}'"
            )
            code = "E001"

        else:
            friendly_msg = f"Syntax Error: {msg}"
            code = "E001"

        error = IssuePrinter(node, friendly_msg, code=code)
        self.issues.append(error)

    def reportAmbiguity(
        self, recognizer, dfa, startIndex, stopIndex, exact, ambigAlts, configs
    ):
        pass
