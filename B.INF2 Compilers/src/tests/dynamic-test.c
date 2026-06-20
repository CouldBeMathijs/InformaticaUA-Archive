#include <stdio.h>

/**
 * A program to print numbers 0-20 and mark a user-selected number.
 * Compatible with ANSI C (C89/C90).
 */
int main() {
  int userInput;
  int i;

  printf("Enter a number between 0 and 20: ");

  if (scanf("%d", &userInput) != 1) {
    printf("Invalid input. Please enter an integer.\n");
    return 1;
  }

  printf("\n--- Results ---\n");

  for (i = 0; i <= 20; i++) {
    printf("%d", i);

    if (i == userInput) {
      printf(" <-");
    }

    printf("\n");
  }

  return 0;
}
