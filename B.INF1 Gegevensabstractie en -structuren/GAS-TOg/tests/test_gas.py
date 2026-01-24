import pathlib
import importlib


def test_gas_bestaat():
    # check of de gas module bestaat
    gas_module = pathlib.Path("../gas")
    assert gas_module.is_dir(), "De gas module bestaat niet"


REQUIRED = {"Bst.py", "MyStack.py", "MyQueue.py"}
ADVANCED = {
    "RedBlackTree.py",
    "TwoThreeTree.py",
    "TwoThreeFourTree.py",
    "Heap.py",
    "Hashmap.py",
}


def test_gas_bestanden():
    # check of elke gas submodule het volgende bevat:
    # minstens verplichte bestanden: Bst.py, MyStack.py en MyQueue.py
    # minstens een uitbreiding: RedBlackTree.py, TwoThreeTree.py, TwoThreeFourTree.py, Heap.py of Hashmap.py
    # geen onbekende bestanden
    for student_directory in iterate_gas_submodules():

        files = {f.name for f in student_directory.glob("*.py")}

        # check verplichte bestanden
        missing_files = REQUIRED - files
        assert (
            len(missing_files) == 0
        ), f"{student_directory} mist verplichte datastructuren: {missing_files}"

        # check uitbreiding
        assert (
            len(ADVANCED & files) > 0
        ), f"{student_directory} bevat geen extra datastructuur {ADVANCED}"

        # check onbekende bestanden
        extra_files = files - (REQUIRED | ADVANCED)
        assert (
            len(extra_files) == 0
        ), f"{student_directory} bevat onbekende bestanden {extra_files}"


def test_gas_implementatie_imports():
    for student_directory in iterate_gas_submodules():
        for file in student_directory.glob("*.py"):
            match file.name:
                case "MyQueue.py":
                    check_class_in_file(file, "MyQueue")
                case "MyStack.py":
                    check_class_in_file(file, "MyStack")
                case "Bst.py":
                    check_class_in_file(file, "BST")
                case "RedBlackTree.py":
                    check_class_in_file(file, "RedBlackTree")
                case "TwoThreeTree.py":
                    check_class_in_file(file, "TwoThreeTree")
                case "TwoThreeFourTree.py":
                    check_class_in_file(file, "TwoThreeFourTree")
                case "Heap.py":
                    check_class_in_file(file, "Heap")
                case "Hashmap.py":
                    check_class_in_file(file, "Hashmap")


def iterate_gas_submodules():
    gas_module = pathlib.Path("../gas")
    for sub_module in gas_module.iterdir():
        if not sub_module.is_dir() or sub_module.name == "__pycache__":
            continue

        yield sub_module


def path_to_module(path: pathlib.Path):
    module_name = (
        str(path.with_suffix(""))
        .replace(".", "")
        .replace("/", ".")
        .replace("\\", ".")[1:]
    )
    return importlib.import_module(module_name)


def check_class_in_file(path: pathlib.Path, class_name):
    mod = path_to_module(path)
    assert hasattr(mod, class_name), f"Geen `{class_name}` klasse gevonden in {path}"
