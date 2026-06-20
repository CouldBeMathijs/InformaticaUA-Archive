// test3_arrays_strings_simpel.c
#include <stdio.h>

int main() {
    // 1. Simpele 1D array met initialisatie
    int arr[3] = {10, 20, 30};

    // 2. Simpele 2D array (verplicht voor project 3)
    int matrix[2][2] = {{1, 2}, {3, 4}};

    // 3. Simpele bewerking op array elementen (met hardcoded indexen ipv variabelen)
    arr[0] = matrix[0][1] + arr[1]; // arr[0] wordt 2 + 20 = 22

    // 4. Strings en printf
    char boodschap[] = "Test programma voor arrays";
    printf("%s\n", boodschap);

    // Printf met een getal uit een array
    printf("Nieuwe waarde in arr[0]: %d\n", arr[0]);

    // 5. Scanf testen
    int input;
    printf("Voer een getal in: ");
    scanf("%d", &input);
    printf("Je typte: %d\n", input);

    return 0;
}
