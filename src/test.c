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

int main(int argc, char **argv) {
  SDM_shift_args(&argc, &argv);
  if (argc > 0) printf("Arguments:\n");
  while (argc > 0) {
    char *argument = SDM_shift_args(&argc, &argv);
    printf("\t%s\n", argument);
  }

  const char *file_path = "./src/test.c";
  char *contents = SDM_read_entire_file(file_path);
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
  SDM_ARRAY_PUSH(dbl_da, 1.0);
  SDM_ARRAY_PUSH(dbl_da, 2.0);
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);
  for (size_t i=0; i<dbl_da.length; i++) {
    printf("Item %zu: %f\n", i, dbl_da.data[i]);
  }

  printf("\nResetting...\n");
  SDM_ARRAY_RESET(dbl_da);
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);
  for (size_t i=0; i<dbl_da.length; i++) {
    printf("Item %zu: %f\n", i, dbl_da.data[i]);
  }

  printf("\nFreeing...\n");
  SDM_ARRAY_FREE(dbl_da);
  printf("dbl_da has a length of %zu\n", dbl_da.length);
  printf("dbl_da has a capacity of %zu\n", dbl_da.capacity);

  printf("\nCreating another empty dyn array...\n");
  char_da_array char_da = {0};
  printf("char_da has a length of %zu\n", char_da.length);
  printf("char_da has a capacity of %zu\n", char_da.capacity);

  printf("\nPushing...\n");
  SDM_ARRAY_PUSH(char_da, 'A');
  SDM_ARRAY_PUSH(char_da, 'B');
  SDM_ARRAY_PUSH(char_da, 'C');
  printf("char_da has a length of %zu\n", char_da.length);
  printf("char_da has a capacity of %zu\n", char_da.capacity);
  for (size_t i=0; i<char_da.length; i++) {
    printf("Item %zu: %c\n", i, char_da.data[i]);
  }
  size_t new_cap = 1024;
  printf("Expanding capacity to at least %zu items...\n", new_cap);
  SDM_ENSURE_ARRAY_MIN_CAP(char_da, new_cap);
  printf("char_da has a length of %zu\n", char_da.length);
  printf("char_da has a capacity of %zu\n", char_da.capacity);

  new_cap = 32;
  printf("shrinking capacity to %zu items...\n", new_cap);
  SDM_ENSURE_ARRAY_CAP(char_da, new_cap);
  printf("char_da has a length of %zu\n", char_da.length);
  printf("char_da has a capacity of %zu\n", char_da.capacity);
  SDM_ARRAY_FREE(char_da);

  printf("\n");
  DblArray hashmap = {0};
  new_dblarray(8, &hashmap);
  push_to_dblarray(&hashmap, "stephen", 69);
  push_to_dblarray(&hashmap, "stephen", 42);
  double test_value = 0;
  if (get_from_hashmap(&hashmap, "stephen", &test_value)) {
    printf("\"stephen\" has a value of %f\n", test_value);
  } else {
    printf("\"stephen\" apparently has no value!\n");
  }
  push_to_dblarray(&hashmap, "a", 'a');
  push_to_dblarray(&hashmap, "b", 'b');
  push_to_dblarray(&hashmap, "c", 'c');
  push_to_dblarray(&hashmap, "d", 'd');
  push_to_dblarray(&hashmap, "e", 'e');
  push_to_dblarray(&hashmap, "f", 'f');
  push_to_dblarray(&hashmap, "g", 'g');

  bool data_integrity_good = true;
  get_from_hashmap(&hashmap, "a", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'a');
  get_from_hashmap(&hashmap, "b", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'b');
  get_from_hashmap(&hashmap, "c", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'c');
  get_from_hashmap(&hashmap, "d", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'd');
  get_from_hashmap(&hashmap, "e", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'e');
  get_from_hashmap(&hashmap, "f", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'f');
  get_from_hashmap(&hashmap, "g", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'g');

  if (data_integrity_good) {
    printf("Hashmap data is good\n");
  } else {
    return 1;
  }

  push_to_dblarray(&hashmap, "z", 3.14159);
  get_from_hashmap(&hashmap, "z", &test_value);
  printf("Expecting 3.14159, got %f\n", test_value);
  printf("The hashmap now has a capacity of %zu\n", hashmap.capacity);

  data_integrity_good = true;
  get_from_hashmap(&hashmap, "a", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'a');
  get_from_hashmap(&hashmap, "b", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'b');
  get_from_hashmap(&hashmap, "c", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'c');
  get_from_hashmap(&hashmap, "d", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'd');
  get_from_hashmap(&hashmap, "e", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'e');
  get_from_hashmap(&hashmap, "f", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'f');
  get_from_hashmap(&hashmap, "g", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 'g');
  get_from_hashmap(&hashmap, "stephen", &test_value);
  data_integrity_good = data_integrity_good && (test_value == 42);

  if (data_integrity_good) {
    printf("Hashmap data is still good\n");
  } else {
    return 1;
  }

  return 0;
}
