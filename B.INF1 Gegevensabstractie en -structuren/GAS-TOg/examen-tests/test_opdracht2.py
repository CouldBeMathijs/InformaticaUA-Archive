from quetzal.app import App

def test_extra_werknemer_tabel():
    try:
        app = App()
    except:
        raise AssertionError("Er ging iets mist met het aanmaken van de App")

    try:
        werknemer_tabel2 = app.get_internals()["werknemer_tabel2"]
    except:
        raise AssertionError("De extra werknemer tabel werd niet teruggegeven met App::get_internals()")
    
    try:
        assert werknemer_tabel2 is not None
        assert werknemer_tabel2.tableLength() == 0
    except:
        raise AssertionError("De extra werknemer tabel werd niet correct aangemaakt")
    
    try:
        app.run("init\nshot melk 10 2030 5 1\nhoning 10 2030 5 1\nwerknemer Jane Doe 5\ngebruiker John Doe john@doe.com\nstart\n1 bestel john@doe.com melk honing 2024 5 1 11 30\n4 log")
    except:
        raise AssertionError(f"Er ging iets mis met het uitvoeren van het script")

    assert werknemer_tabel2.tableLength() == 1, "De extra werknemer tabel werd niet correct gebruikt"


def test_voornaam_of_achternaam():
    try:
        app = App()
    except:
        raise AssertionError("Er ging iets mist met het aanmaken van de App")

    try:
        werknemer_tabel1 = app.get_internals()["werknemer_tabel"]
        werknemer_tabel2 = app.get_internals()["werknemer_tabel2"]
    except:
        raise AssertionError("De extra werknemer tabel werd niet teruggegeven met App::get_internals()")
    
    try:
        app.run("init\nshot melk 10 2030 5 1\nhoning 10 2030 5 1\nwerknemer Jane Doe 5\ngebruiker John Doe john@doe.com\nstart\n1 bestel john@doe.com melk honing 2024 5 1 11 30\n4 log")
    except:
        raise AssertionError("Er ging iets mis met het uitvoeren van het script")

    try:
        assert werknemer_tabel1.tableRetrieve("Jane")[1] or werknemer_tabel1.tableRetrieve("Doe")[1]
        assert werknemer_tabel2.tableRetrieve("Jane")[1] or werknemer_tabel2.tableRetrieve("Doe")[1]
    except:
        raise AssertionError("De werknemer tabellen gebruiken als zoeksleutel niet voornaam of achternaam.")

