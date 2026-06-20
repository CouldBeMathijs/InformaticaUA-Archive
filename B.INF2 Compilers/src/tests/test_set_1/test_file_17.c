#include <stdio.h>
int main() {

char x = 'a';

char* chr_ptr = &x;

*chr_ptr = 'b';

char another_char = *chr_ptr;
	printf("%d\n", another_char);
	printf("%d\n", x);
}
