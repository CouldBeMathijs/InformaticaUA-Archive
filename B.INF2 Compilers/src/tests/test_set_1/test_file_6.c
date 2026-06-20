#include <stdio.h>
int main() {
    int x = 4;
    int y = 5;

    int* ptr = &x;
    ptr++;
    ptr--;

    int is_x = (ptr == &x);
    int is_y = (ptr == &y);
    is_y = (&x != ptr);

    float* ptr2 = 0;

    ptr2 >= ptr;
    ptr2 <= ptr;
    ptr > &x;
    ptr < 32;

    int num_skip_elements = 4;

    ptr = ptr + 4*num_skip_elements;
	printf("%d\n", is_x);
	printf("%d\n", is_y);
	printf("%d\n", num_skip_elements);
	printf("%d\n", x);
	printf("%d\n", y);
}
