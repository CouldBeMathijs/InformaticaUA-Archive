#include <stdio.h>
int main() {
int x = 5;
x--;

int z = x--;
x = x-- + z--;

--x;
	printf("%d\n", x);
	printf("%d\n", z);
}
