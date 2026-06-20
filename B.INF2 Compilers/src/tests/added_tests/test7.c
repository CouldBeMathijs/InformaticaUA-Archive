#include <stdio.h>
#include <stdlib.h>

/* Define a struct to hold our data */
struct Record {
  int id;
  float score;
  char *name;
};

/* Manual string length using int */
int manual_strlen(char *s) {
  int len = 0;
  while (s[len] != '\0') {
    len++;
  }
  return len;
}

/* Manual string copy */
void manual_strcpy(char *dest, char *src) {
  int i = 0;
  while (src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
}

int main() {
  int i;
  int num_elements = 3;
  struct Record *array;
  /* Removed 'const' to avoid E103 read-only errors */
  char *names[3];

  names[0] = "Alpha";
  names[1] = "Bravo";
  names[2] = "Charlie";

  /* 1. Allocate array of structs */
  array = (struct Record *)malloc(num_elements * sizeof(struct Record));

  /* Replaced NULL with 0 for E101 errors */
  if (array == 0) {
    return 1;
  }

  /* 2. Initialize the array */
  for (i = 0; i < num_elements; i++) {
    int name_len = manual_strlen(names[i]);

    array[i].id = i + 1;
    array[i].score = (float)i * 1.5;

    /* Allocate memory for the string */
    array[i].name = (char *)malloc((name_len + 1) * sizeof(char));

    if (array[i].name != 0) {
      manual_strcpy(array[i].name, names[i]);
    }
  }

  /* 3. Output results */
  printf("ID   Score    Name\n");
  printf("--------------------\n");
  for (i = 0; i < num_elements; i++) {
    printf("%d    %.2f    ", array[i].id, array[i].score);

    if (array[i].name != 0) {
      printf("%s\n", array[i].name);
    } else {
      printf("None\n");
    }
  }

  /* 4. Cleanup memory */
  for (i = 0; i < num_elements; i++) {
    if (array[i].name != 0) {
      free(array[i].name);
    }
  }
  free(array);

  return 0;
}
