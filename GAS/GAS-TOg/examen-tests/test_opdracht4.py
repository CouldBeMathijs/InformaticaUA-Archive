from quetzal.Milkshake import milkshake_scenario

def test_milkshake_prijs():
    try:
        assert milkshake_scenario("small", 1, 2) == 2.50
        assert milkshake_scenario("medium", 5, 2) == 7.50
        assert milkshake_scenario("large", 1, 3) == 4.75
    except:
        raise AssertionError("De prijs berekening voor milkshakes is niet correct")
