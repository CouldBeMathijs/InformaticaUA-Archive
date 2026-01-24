def item_order(unorder):
    s = 0; h = 0; w = 0
    for i in unorder:
        if i == "s":
            s += 1
        elif i == "h":
            h += 1
        elif i == "w":
            w += 1
        else:
            return "Error"
    return [s, h, w]
# print(item_order(["h", "w", "h"]))
# print(item_order(["s", "w", "h", "s", "h"]))
