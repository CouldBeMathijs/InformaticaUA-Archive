#include <stdio.h>
int main() {

int x = 54;

int z = -33;

int* p = &z;

x = *p;

p* = 5;
	printf("%d\n", x);
	printf("%d\n", z);
}
