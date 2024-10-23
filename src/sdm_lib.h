#ifndef _SDM_LIB_H
#define _SDM_LIB_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define hash jenkins_one_at_a_time_hash

#define SDM_ENSURE_ARRAY_CAP(da, cap) do {                     \
    (da).capacity = cap;                                       \
    (da).data = realloc((da).data,                             \
        (da).capacity * sizeof((da).data[0]));                 \
    if ((da).data == NULL) {                                   \
      fprintf(stderr, "ERR: Couldn't alloc memory.\n");        \
      exit(1);                                                 \
    }                                                          \
  } while (0)

#define SDM_ENSURE_ARRAY_MIN_CAP(da, cap) do {                 \
    if ((da).capacity < cap) {                                 \
      (da).capacity = cap;                                     \
      (da).data = realloc((da).data,                           \
          (da).capacity * sizeof((da).data[0]));               \
      if ((da).data == NULL) {                                 \
        fprintf(stderr, "ERR: Couldn't alloc memory. \n");     \
        exit(1);                                               \
      }                                                        \
    }                                                          \
  } while (0)

#define SDM_ARRAY_PUSH(da, item) do {                          \
    if (((da).capacity == 0) || ((da).data == NULL)) {         \
      (da).capacity = DEFAULT_CAPACITY;                        \
      (da).data = calloc((da).capacity, sizeof((da).data[0])); \
      if ((da).data == NULL) {                                 \
        fprintf(stderr, "ERR: Couldn't alloc memory.\n");      \
        exit(1);                                               \
      }                                                        \
    }                                                          \
    while ((da).length >= (da).capacity) {                     \
      (da).capacity *= 2;                                      \
      (da).data = realloc((da).data,                           \
           (da).capacity * sizeof((da).data[0]));              \
      if ((da).data == NULL) {                                 \
        fprintf(stderr, "ERR: Couldn't alloc memory.\n");      \
        exit(1);                                               \
      }                                                        \
    }                                                          \
    (da).data[(da).length++] = item;                           \
  } while (0);

#define SDM_ARRAY_FREE(da) do {                                \
    free((da).data);                                           \
    (da).length = 0;                                           \
    (da).capacity = 0;                                         \
  } while (0);

#define SDM_ARRAY_RESET(da) do {                               \
    (da).length = 0;                                           \
  } while (0)

typedef struct {
  size_t length;
  char *data;
} SDM_StringView;

#define SDM_SV_F "%.*s"
#define SDM_SV_Vals(S) (int)(S).length, (S).data

char *SDM_read_entire_file(const char *file_path);

SDM_StringView SDM_cstr_as_sv(char *cstr);
SDM_StringView SDM_sized_str_as_sv(char *cstr, size_t length);

SDM_StringView SDM_sv_pop_by_delim(SDM_StringView *SV, const char delim);
void SDM_sv_trim(SDM_StringView *SV);

char *SDM_shift_args(int *argc, char ***argv);

typedef struct {
  char key[32];
  double value;
  bool occupied;
} Dbl_KVPair;

typedef struct {
  Dbl_KVPair *data;
  size_t capacity;
} DblArray;

DblArray new_dblarray(size_t cap);
void push_to_dblarray(DblArray *hm, char *key, double value);
bool get_from_hashmap(DblArray *hm, char *key, double *retval);
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);

#endif /* ifndef _SDM_LIB_H */

