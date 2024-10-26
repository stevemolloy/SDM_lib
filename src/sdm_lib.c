#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sdm_lib.h"

char *SDM_read_entire_file(const char *file_path) {
  // Reads an entire file into a char array, and returns a ptr to this. The ptr should be freed by the caller
  FILE *f = fopen(file_path, "r");
  if (f==NULL) {
    fprintf(stderr, "Could not read %s: %s\n", file_path, strerror(errno));
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0L, SEEK_SET);

  char *contents = calloc(sz + 1, sizeof(char));
  if (contents==NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    exit(1);
  }
  fread(contents, 1, sz, f);

  fclose(f);
  
  return contents;
}

SDM_StringView SDM_cstr_as_sv(char *cstr) {
  return (SDM_StringView){
    .data = cstr,
    .length = strlen(cstr)
  };
}

SDM_StringView SDM_sized_str_as_sv(char *cstr, size_t length) {
  return (SDM_StringView) {
    .data = cstr,
    .length = length
  };
}

SDM_StringView SDM_sv_pop_by_delim(SDM_StringView *SV, const char delim) {
  SDM_StringView ret = {0};
  ret.data = SV->data;

  while ((*SV->data != delim) && (SV->length>0)) {
    SV->data++;
    SV->length--;
    ret.length++;
  }
  SV->data++;
  SV->length--;

  return ret;
}

void SDM_sv_trim(SDM_StringView *SV) {
  while (isspace(*SV->data) && SV->length>0) {
    SV->data++;
    SV->length--;
  }
}

char *SDM_shift_args(int *argc, char ***argv) {
  if (*argc <= 0) return NULL;
  (*argc)--;
  char **ret = *argv;
  (*argv)++;
  return *ret;
}

#define SET_HM_CAPACITY(hm, cap)                            \
  do {                                                      \
    (hm)->capacity = cap;                                   \
    (hm)->data = calloc(hm->capacity, sizeof(hm->data[0])); \
    if ((hm)->data == NULL) {                               \
      fprintf(stderr, "ERR: Can't alloc.\n");               \
      exit(1);                                              \
    }                                                       \
  } while (0)

uint32_t get_hashmap_location(const char* key, size_t capacity) {
  uint32_t key_hash = hash((uint8_t*)key, strlen(key));
  return key_hash % capacity;
}

#define PUSH_TO_HASHMAP(hm1, value_of_key, value_of_value)                                    \
  do {                                                                                       \
    uint32_t location;                                                           \
    if ((hm1)->capacity == 0) {                                                               \
      SET_HM_CAPACITY((hm1), DEFAULT_HM_CAP);                                                 \
    }                                                                                        \
    location = get_hashmap_location(value_of_key, (hm1)->capacity);                                    \
                                                                                             \
    resize_needed = true;                                                                    \
    if (!hm1->data[location].occupied ||                                                      \
      (hm1->data[location].occupied && strcmp(value_of_key, hm1->data[location].key)==0)) {             \
      strcpy(hm1->data[location].key, value_of_key);                                                   \
      hm1->data[location].value = value_of_value;                                                      \
      hm1->data[location].occupied = true;                                                    \
      resize_needed = false;                                                                 \
    } else {                                                                                 \
      for (size_t i=1; i<hm1->capacity; i++) {                                                \
        size_t new_location = (location + i) % hm1->capacity;                                 \
        if (!hm1->data[new_location].occupied ||                                              \
          (hm1->data[new_location].occupied && strcmp(value_of_key, hm1->data[new_location].key)==0)) { \
          strcpy(hm1->data[new_location].key, value_of_key);                                           \
          hm1->data[new_location].value = value_of_value;                                              \
          hm1->data[new_location].occupied = true;                                            \
          resize_needed = false;                                                             \
          break;                                                        \
        }                                                                                    \
      }                                                                                      \
    }                                                                                        \
  } while (0)

void push_to_dblarray(DblArray *hm, char *key, double value) {
  bool resize_needed;
  PUSH_TO_HASHMAP(hm, key, value);
  
  if (resize_needed) {
    DblArray *resized_array = calloc(1, sizeof(*hm));
    if (resized_array == NULL) {
      fprintf(stderr, "ERR: Can't alloc.\n");
      exit(1);
    }
    SET_HM_CAPACITY(resized_array, hm->capacity * 2);
    for (size_t i=0; i<hm->capacity; i++) {
      if (!hm->data[i].occupied) continue;
      push_to_dblarray(resized_array, hm->data[i].key, hm->data[i].value);
    }
    push_to_dblarray(resized_array, key, value);
    hm->capacity = resized_array->capacity;
    free(hm->data);
    hm->data = resized_array->data;
    free(resized_array);
  }
}

// https://en.wikipedia.org/wiki/Jenkins_hash_function
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

