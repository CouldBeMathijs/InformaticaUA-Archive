from Oefening03 import item_order
db = {}
def place_order(db,email,order):
    order = item_order(order)
    db[email] = order

#tests
#(db,"jefke@gmail.com",["s", "w", "h", "s", "h"])
#print(db)