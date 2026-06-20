#include <stdio.h>
#include <stdlib.h>

/* Define a simple struct for testing */
struct Item {
  int code;
  float value;
  char tag;
};

int main() {
  struct Item *item_array;
  struct Item *ptr;
  int i;
  int count = 4;

  /* Allocate a single block of memory for 4 structs */
  item_array = (struct Item *)malloc(count * sizeof(struct Item));

  if (item_array == 0) {
    return 1;
  }

  /* Test 1: Initialize using standard array indexing */
  for (i = 0; i < count; i++) {
    item_array[i].code = i * 10;
    item_array[i].value = (float)i + 0.5;
    /* Cycle through characters A, B, C, D */
    item_array[i].tag = (char)(65 + i);
  }

  /* Test 2: Read back using pointer arithmetic */
  /* This verifies the memory is contiguous and aligned */
  printf("Testing Struct Array Layout:\n");
  printf("Index | Code | Value | Tag | Address Offset\n");
  printf("------------------------------------------\n");

  for (i = 0; i < count; i++) {
    /* Set ptr to the i-th element using pointer math */
    ptr = item_array + i;

    printf("%d     | %d   | %.1f   | %c   | +%d bytes\n", i, ptr->code,
           ptr->value, ptr->tag, (int)((char *)ptr - (char *)item_array));
  }

  /* Test 3: Modification via pointer */
  ptr = item_array;
  ptr[2].code = 999; /* Change the 3rd element's code */

  if (item_array[2].code == 999) {
    printf("\nPointer modification test: PASSED\n");
  } else {
    printf("\nPointer modification test: FAILED\n");
  }

  /* Clean up the entire array with one free */
  free(item_array);

  return 0;
}
