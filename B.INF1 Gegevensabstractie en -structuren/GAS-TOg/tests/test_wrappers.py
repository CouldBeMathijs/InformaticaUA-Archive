from tests.test_gas import check_class_in_file, iterate_gas_submodules


def test_gas_wrapper_imports():
    for student_directory in iterate_gas_submodules():
        for file in student_directory.glob("*.py"):
            match file.name:
                case "Bst.py":
                    check_class_in_file(file, "BSTTable")
                case "RedBlackTree.py":
                    check_class_in_file(file, "RedBlackTreeTable")
                case "TwoThreeTree.py":
                    check_class_in_file(file, "TwoThreeTreeTable")
                case "TwoThreeFourTree.py":
                    check_class_in_file(file, "TwoThreeFourTreeTable")
                case "Heap.py":
                    check_class_in_file(file, "HeapQueue")
                case "Hashmap.py":
                    check_class_in_file(file, "HashmapTable")
