from gas.student1.Bst as Bststudentname
def run_general_tests(table):
    """Runs a set of generic tests on the given table implementation with interleaved operations."""

    assert table.tableIsEmpty()

    # Insert some entries
    assert table.tableInsert((5, 5))
    assert table.tableInsert((3, "Hoi"))
    assert table.tableInsert((8, 3.14))
    assert table.tableInsert((10, (1, 2, 3)))

    # Retrieve and delete one early
    assert table.tableRetrieve(3) == ("Hoi", True)
    assert table.tableDelete(3) is True
    assert not table.tableDelete(3)  # Already deleted

    # More insertions
    assert table.tableInsert((12, ["a", "b", "c"]))
    assert table.tableInsert((15, {"key": "value"}))

    # Delete an earlier one
    assert table.tableDelete(5) is True

    # Continue inserting
    assert table.tableInsert((18, True))
    assert table.tableInsert((20, None))

    # Now check length (only 6 remain)
    assert table.tableLength() == 6

    # Retrieval tests for remaining keys
    assert table.tableRetrieve(8) == (3.14, True)
    assert table.tableRetrieve(10) == ((1, 2, 3), True)
    assert table.tableRetrieve(12) == (["a", "b", "c"], True)
    assert table.tableRetrieve(15) == ({"key": "value"}, True)
    assert table.tableRetrieve(18) == (True, True)
    assert table.tableRetrieve(20) == (None, True)

    # Delete mid-phase entries
    assert table.tableDelete(10) is True
    assert table.tableDelete(18) is True

    # Try re-inserting a deleted key
    assert table.tableInsert((10, "Reinserted")) is True
    assert table.tableRetrieve(10) == ("Reinserted", True)

    # Traverse keys and values
    keys = []
    table.traverseTableKeys(keys.append)
    keys.sort()
    assert keys == [8, 10, 12, 15, 20]

    values = []
    table.traverseTableValues(values.append)
    for expected in [3.14, "Reinserted", ["a", "b", "c"], {"key": "value"}, None]:
        assert expected in values
        values.remove(expected)
    assert not values  # All matched

    # Delete rest of the keys
    for key in [8, 12, 15, 20, 10]:
        assert table.tableDelete(key) is True

    assert table.tableIsEmpty()

    # Final batch test — edge case: reinsert and overwrite fail
    assert table.tableInsert((1, "one")) is True
    assert not table.tableInsert((1, "uno"))  # Should fail (duplicate key)
    assert table.tableRetrieve(1) == ("one", True)

    # Delete and reinstate
    assert table.tableDelete(1)
    assert table.tableInsert((1, "uno")) is True
    assert table.tableRetrieve(1) == ("uno", True)

    assert table.tableDelete(1)
    assert table.tableIsEmpty()


def test_bst_studentname():
    run_general_tests(Bststudentname())
