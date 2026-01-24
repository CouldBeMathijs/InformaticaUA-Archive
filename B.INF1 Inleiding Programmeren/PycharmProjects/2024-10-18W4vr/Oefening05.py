from Oefening03 import item_order
from Oefening04 import place_order
db = {}
place_order(db,"jefke@gmail.com",["s", "w", "h", "s", "h"])
def print_previous_order(db,email):
    if email in db:
        print(f"Je bestelde {db[email][0]} salade(s).")
        print(f"Je bestelde {db[email][1]} hamburger(s).")
        print(f"Je bestelde {db[email][2]} water(s).")
    else:
        print("Je hebt nog geen bestellingen geplaatst.")

print_previous_order(db,"jefke@gmail.com")