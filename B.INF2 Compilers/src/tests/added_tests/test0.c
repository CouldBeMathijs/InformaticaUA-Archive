#include <stdio.h>

int main() {
  const int x = 4;
  int y = 5;

  int *ptr = &x;
  ptr++;
  ptr--;

  int is_x = (ptr == &x);
  int is_y = (ptr == &y);
  is_y = (&x != ptr);

  float *ptr2 = 0;
  int num_skip_elements = 4;

  ptr = ptr + 4 * num_skip_elements;

  float myfloat = 10;
  myfloat = 5;
  myfloat = 5;

  int myint = 3;
  myint = 'a';
  char mychar = myint;
  const int x2 = 78;
  const int myabc = x + 3 * 4; /*This is a comment*/
  int qwerty = 2 * myabc;
  const int x3 = x;
  char kul = '\n';
  int ages[5] = {4, 3, 2, 1, 0};
  printf("%d", qwerty);
}
