#include <stdio.h>
int main() {

int x = 478;
int b = -251454;

int** x_ptr = &x;
x_ptr = &b;
	printf("%d\n", b);
	printf("%d\n", x);
}
