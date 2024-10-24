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

void new_dblarray(size_t cap, DblArray *hm) {
  hm->capacity = cap;
  hm->data = calloc(hm->capacity, sizeof(hm->data[0]));
  if (hm->data == NULL) {
    fprintf(stderr, "ERR: Can't alloc.\n");
    exit(1);
  }
}

void push_to_dblarray(DblArray *hm, char *key, double value) {
  uint32_t key_hash = hash((uint8_t*)key, strlen(key));
  uint32_t location = key_hash % hm->capacity;
  
  if (!hm->data[location].occupied || (hm->data[location].occupied && strcmp(key, hm->data[location].key)==0)) {
    strcpy(hm->data[location].key, key);
    hm->data[location].value = value;
    hm->data[location].occupied = true;
  } else {
    for (size_t i=1; i<hm->capacity; i++) {
      size_t new_location = (location + i) % hm->capacity;
      if (!hm->data[new_location].occupied || (hm->data[new_location].occupied && strcmp(key, hm->data[new_location].key)==0)) {
        strcpy(hm->data[new_location].key, key);
        hm->data[new_location].value = value;
        hm->data[new_location].occupied = true;
        return;
      }
    }
    size_t new_capacity = hm->capacity * 2;
    DblArray *resized_array = calloc(1, sizeof(DblArray));
    new_dblarray(new_capacity, resized_array);
    for (size_t i=0; i<hm->capacity; i++) {
      if (!hm->data[i].occupied) continue;
      push_to_dblarray(resized_array, hm->data[i].key, hm->data[i].value);
    }
    push_to_dblarray(resized_array, key, value);
    hm->capacity = resized_array->capacity;
    hm->data = resized_array->data;
  }
}

bool get_from_hashmap(DblArray *hm, char *key, double *retval) {
  uint32_t key_hash = hash((uint8_t*)key, strlen(key));
  uint32_t location = key_hash % hm->capacity;

  for (size_t i=0; i<hm->capacity; i++) {
    size_t new_location = (location + i) % hm->capacity;
    if (hm->data[new_location].occupied && strcmp(hm->data[new_location].key, key)==0) {
      *retval = hm->data[new_location].value;
      return true;
    }
  }
  return false;
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

