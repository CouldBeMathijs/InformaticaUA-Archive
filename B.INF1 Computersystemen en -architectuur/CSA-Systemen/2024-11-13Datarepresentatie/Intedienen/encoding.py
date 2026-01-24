from files import *
# a, lees het bestand in als utf-8
inputbestand = read_file("input.txt", en=UTF_8).splitlines()

# b, schrijf het bestand weg als utf-16
write_file("text_in_UTF_16.txt", "".join(inputbestand), en=UTF_16)

# c, maak de nieuwe variabele code_points, en schrijf die weg naar het bestand
code_points = []
for line in inputbestand:
    # Voor elke lijn, voeg die toe als codepoints aan de code_points lijst
    for char in line:
        code_points.append(f"{ord(char):04X}")
    code_points.append("")  # Dit stelt een nieuwe lijn voor

# Schrijf de code_points naar een bestand, met spaties er tussen
write_file("code_points.txt", " ".join(code_points), en=UTF_8)

# d, en nu nog naar HTML
html_content = []
for code_point in code_points:
    if code_point == "":  # Dit stelde een nieuwe lijn voor, dus vervang die door een <br>
        html_content.append("<br>")
    else:
        html_content.append(f"&#x{code_point};")
write_html_file("text_in_HTML.html", "".join(html_content))
print("Done :)")
