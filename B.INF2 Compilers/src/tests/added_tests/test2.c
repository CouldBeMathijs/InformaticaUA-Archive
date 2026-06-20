#include <stdio.h>

int printListWithSelector(int max, int selected) {
  int i;
  for (i = 0; i <= max; i++) {
    printf("%d", i);

    if (i == selected) {
      printf(" <-");
    }

    printf("\n");
  }
  return 0;
}

int main() {
  int userInput;
  int i;

  printf("\n--- Results ---\n");

  printListWithSelector(40, 7);

  return 0;
}
