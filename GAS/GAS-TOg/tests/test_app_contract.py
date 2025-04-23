def test_run():
    from quetzal.app import App

    # check of de App.run methode bestaat
    assert hasattr(App, "run"), "App heeft geen run methode"

    # check of de methode een dict teruggeeft
    app = App()
    resulaten = app.run("")
    assert isinstance(resulaten, dict), "App.run geeft geen dictionary terug"


def test_get_internals():
    from quetzal.app import App

    # check of de App.get_internals methode bestaat
    assert hasattr(App, "get_internals"), "App heeft geen get_internals methode"

    # check of de methode een dict teruggeeft
    app = App()
    internals = app.get_internals()
    assert isinstance(internals, dict), "App.get_internals geeft geen dictionary terug"

    # check of de interne datastructuren uit de gas module komen uit een bestand met juiste naam

    VALID_TABELS = [
        "Bst",
        "RedBlackTree",
        "TwoThreeTree",
        "TwoThreeFourTree",
        "Hashmap",
    ]
    VALID_QUEUES = ["MyQueue", "Heap"]
    VALID_STACKS = ["MyStack"]

    check_object(internals, "gebruiker_tabel", VALID_TABELS)
    check_object(internals, "werknemer_tabel", VALID_TABELS)
    check_object(internals, "bestelling_tabel", VALID_TABELS)
    check_object(internals, "honing_stock", VALID_TABELS)
    check_object(internals, "chilipeper_stock", VALID_TABELS)
    check_object(internals, "marshmallow_stock", VALID_TABELS)
    check_object(internals, "bestelling_queue", VALID_QUEUES)
    check_object(internals, "werknemer_stack", VALID_STACKS)

    # check of de output dezelfde objecten zijn
    app.run("")
    internals2 = app.get_internals()
    for v1, v2 in zip(internals.values(), internals2.values()):
        assert v1 is v2, "App.get_internals geeft niet altijd dezelfde objecten terug"


def check_object(internals, name, allowed):
    object = internals[name]
    assert object is not None, f"{name} is None"
    module = object.__class__.__module__
    m = module.split(".")
    prefix = f"{name} implementatie ({module})"
    assert module.startswith(
        "gas."
    ), f"{prefix} is geen GAS ADT (want zit niet in de gas map)"
    assert (
        m[-1] in allowed
    ), f"{prefix} is niet de juiste GAS ADT (moet een zijn uit {allowed})"
