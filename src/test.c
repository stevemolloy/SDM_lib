#include <stdio.h>
#include <stdlib.h>

#include "sdm_lib.h"

#define DEFAULT_CAPACITY 128

typedef struct {
  double *data;
  size_t length;
  size_t capacity;
} double_da_array;

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
} char_da_array;

int main(void) { const char *file_path = "./src/test.c";
  char *contents = SDM_read_entire_file(file_path);
  printf("%s\n", contents);
  free(contents);

  char *multi_line_str = "Hello\nWorld\nBlahblah\n";
  SDM_StringView multi_line_sv = SDM_cstr_as_sv(multi_line_str);
  SDM_StringView line_1 = SDM_sv_pop_by_delim(&multi_line_sv, '\n');
  SDM_StringView line_2 = SDM_sv_pop_by_delim(&multi_line_sv, '\n');
  printf("Line 1: "SDM_SV_F"\n", SDM_SV_Vals(line_1));
  printf("Line 2: "SDM_SV_F"\n", SDM_SV_Vals(line_2));

  char *test_trim = "   Hello!";
  SDM_StringView test_trim_sv = SDM_cstr_as_sv(test_trim);
  SDM_sv_trim(&test_trim_sv);
  printf("\n");
  printf("Untrimmed string = %s\n", test_trim);
  printf("Trimmed string = "SDM_SV_F"\n", SDM_SV_Vals(test_trim_sv));

  printf("\nCreating empty dyn array...\n");
  double_da_array dbl_da = {0};
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);
  for (size_t i=0; i<dbl_da.length; i++) {
    printf("Item %zu: %f\n", i, dbl_da.data[i]);
  }

  printf("\nPushing...\n");
  ARRAY_PUSH(&dbl_da, 1.0);
  ARRAY_PUSH(&dbl_da, 2.0);
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);
  for (size_t i=0; i<dbl_da.length; i++) {
    printf("Item %zu: %f\n", i, dbl_da.data[i]);
  }

  printf("\nResetting...\n");
  ARRAY_RESET(dbl_da);
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);
  for (size_t i=0; i<dbl_da.length; i++) {
    printf("Item %zu: %f\n", i, dbl_da.data[i]);
  }

  printf("\nFreeing...\n");
  ARRAY_FREE(dbl_da);
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);
  for (size_t i=0; i<dbl_da.length; i++) {
    printf("Item %zu: %f\n", i, dbl_da.data[i]);
  }

  printf("\nCreating another empty dyn array...\n");
  char_da_array dbl_char = {0};
  printf("dbl_char has a length of %zu\n", dbl_char.length);
  printf("dbl_char has a capacity of %zu\n", dbl_char.capacity);
  for (size_t i=0; i<dbl_char.length; i++) {
    printf("Item %zu: %c\n", i, dbl_char.data[i]);
  }

  printf("\nPushing...\n");
  ARRAY_PUSH(&dbl_char, 'A');
  ARRAY_PUSH(&dbl_char, 'B');
  ARRAY_PUSH(&dbl_char, 'C');
  printf("dbl_char has a length of %zu\n", dbl_char.length);
  printf("dbl_char has a capacity of %zu\n", dbl_char.capacity);
  for (size_t i=0; i<dbl_char.length; i++) {
    printf("Item %zu: %c\n", i, dbl_char.data[i]);
  }

  return 0;
}