## Build Instructions

`cmake . && make`

## Run instructions
`./engine {optional: ini file}`
If no .ini file was given, the program will look for the `filelist` and run for all files listed there in. (Note that relative paths within this file are explicitly not supported)

## Gequoteerde functionaliteit
V: Werkend  
-: Deels werkend met gekende problemen (onderaan beschreven)  
X: Niet werkend of niet geïmplementeerd  
(blanco): TODO  


|   | Functionaliteit          | Status |
|---|--------------------------|--------|
| 1 | 2D L-systemen            | V      |
|   | Met haakjes              | V      |
|   | Stochastisch             | V      |
| 2 | Transformaties           | V      |
|   | Eye-point                | V      |
|   | Projectie                | V      |
| 3 | Platonische Lichamen     | V      |
|   | Kegel en cylinder        | V      |
|   | Bol                      | V      |
|   | Torus                    | V      |
|   | 3D L-systemen            | V      |
| 4 | Z-buffering (lijnen)     | V      |
| 5 | Triangulatie             | V      |
|   | Z-buffering (driehoeken) | V      |
| 6 | 3D fractalen             | V      |
|   | BuckyBall                |        |
|   | Mengerspons              |        |
|   | View Frustum             |        |
| 7 | Ambient licht            |        |
|   | Diffuus licht (oneindig) |        |
|   | Diffuus licht (puntbron) |        |
|   | Speculair licht          |        |
| 8 | Schaduw                  |        |
|   | Texture mapping          |        |
| 9 | Bollen en cylinders      |        |
|   | UV-coordinaten           |        |
|   | Cube mapping             |        |

Geïmplementeerde vorm van texture mapping: ...

## Gekende problemen 
...
## Niet-gequoteerde functionaliteit
- Opgave 0: Intro
	- IntroColorRectangle
	- IntroBlocks
	- IntroLines

## Extra functionaliteit, niet in de opgaves beschreven
### Voorbeeld van Stochastisch L2DSystem formaat
```
Alphabet = {F}

Draw = {
       F -> 1
}

Rules = {
    F -> {0.33:"F(+F)F(-F)F",
          0.33:"F(+F)F",
          0.33:"F(-F)F"}
}

Initiator     = "F"
Angle         = 25.7
StartingAngle = 90
Iterations    = 6
```

