#include <stdio.h>

int print(int a);

int main() {
  int a = 4;
  while (a > -2) {
    print(a);
    a--;
  }
}

int print(int a) {
  printf("%d", a);
  switch (a) {
  case 0:
    printf("Zero\n");
  case 1:
    printf("Zero or one\n");
    break;
  case 2:
    printf("Two\n");
    break;
  default:
    printf("Not 0, 1 or 2\n");
  }
  return 0;
}
