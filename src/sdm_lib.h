#ifndef _SDM_LIB_H
#define _SDM_LIB_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

#define SDM_ARRAY_PUSH(da, item) do {                             \
    if (((da).capacity == 0) || ((da).data == NULL)) {            \
      (da).capacity = DEFAULT_CAPACITY;                           \
      (da).data = malloc((da).capacity * sizeof((da).data[0]));   \
      memset((da).data, 0, (da).capacity * sizeof((da).data[0])); \
      if ((da).data == NULL) {                                    \
        fprintf(stderr, "ERR: Couldn't alloc memory.\n");         \
        exit(1);                                                  \
      }                                                           \
    }                                                             \
    while ((da).length >= (da).capacity) {                        \
      (da).capacity *= 2;                                         \
      (da).data = realloc((da).data,                              \
           (da).capacity * sizeof((da).data[0]));                 \
      if ((da).data == NULL) {                                    \
        fprintf(stderr, "ERR: Couldn't alloc memory.\n");         \
        exit(1);                                                  \
      }                                                           \
    }                                                             \
    (da).data[(da).length++] = item;                              \
  } while (0);

#define SDM_ARRAY_FREE(da) do {                                \
    free((da).data);                                           \
    (da).length = 0;                                           \
    (da).capacity = 0;                                         \
  } while (0);

#define SDM_ARRAY_RESET(da) do { (da).length = 0; } while (0)

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
  size_t length;
  size_t capacity;
} DblArray;

#define DEFAULT_HM_CAP 256

#define SET_HM_CAPACITY(hm, cap)                                              \
  do {                                                                        \
    (hm)->capacity = (cap);                                                   \
    if ((hm)->data == NULL) {                                                 \
      (hm)->data = malloc(hm->capacity * sizeof(hm->data[0]));                \
    }                                                                         \
    else {                                                                    \
      (hm)->data = realloc((hm)->data, (hm)->capacity * sizeof(hm->data[0])); \
    }                                                                         \
    if ((hm)->data == NULL) {                                                 \
      fprintf(stderr, "ERR: Can't alloc.\n");                                 \
      exit(1);                                                                \
    }                                                                         \
    memset((hm)->data, 0, (hm)->capacity * sizeof(hm->data[0]));              \
  } while (0)

#define FREE_HASHMAP(hm) \
  do {                   \
    (hm).capacity = 0;   \
    free((hm).data);     \
  } while (0);

#define GET_HASHMAP_INDEX(hm, value_of_key, index_addr)                      \
  do {                                                                       \
    *(index_addr) = -1;                                                      \
    uint32_t location = get_hashmap_location((value_of_key), (hm).capacity); \
    for (size_t offset=0; offset<(hm).capacity; offset++) {                  \
      int new_location = (location + offset) % (hm).capacity;                \
      if ((hm).data[new_location].occupied &&                                \
        (strcmp((hm).data[new_location].key, (value_of_key))==0)) {          \
        *(index_addr) = new_location;                                        \
        break;                                                               \
      }                                                                      \
    }                                                                        \
  } while (0)

#define HM_VAL_AT(hm, index) \
  (hm).data[index].value

#define PUSH_TO_HASHMAP(hm1, value_of_key, value_of_value)                                   \
  do {                                                                                       \
    uint32_t location;                                                                       \
    if ((hm1)->capacity == 0) {                                                              \
      SET_HM_CAPACITY((hm1), DEFAULT_HM_CAP);                                                \
    }                                                                                        \
    if ((hm1)->length >= (hm1)->capacity) {                                                  \
      fprintf(stderr, "Hashmap has insufficient capacity.\n");                               \
      exit(1);                                                                               \
    }                                                                                        \
    location = get_hashmap_location((value_of_key), (hm1)->capacity);                        \
                                                                                             \
    if (!(hm1)->data[location].occupied ||                                                   \
      ((hm1)->data[location].occupied && strcmp((value_of_key), (hm1)->data[location].key)==0)) {  \
      strcpy((hm1)->data[location].key, (value_of_key));                                     \
      (hm1)->data[location].value = (value_of_value);                                        \
      (hm1)->data[location].occupied = true;                                                 \
      (hm1)->length++;                                                                       \
    } else {                                                                                 \
      for (size_t i=1; i<(hm1)->capacity; i++) {                                             \
        size_t new_location = (location + i) % (hm1)->capacity;                              \
        if (!(hm1)->data[new_location].occupied ||                                           \
          ((hm1)->data[new_location].occupied && strcmp((value_of_key), (hm1)->data[new_location].key)==0)) { \
          strcpy((hm1)->data[new_location].key, (value_of_key));                             \
          (hm1)->data[new_location].value = (value_of_value);                                \
          (hm1)->data[new_location].occupied = true;                                         \
          (hm1)->length++;                                                                   \
          break;                                                                             \
        }                                                                                    \
      }                                                                                      \
    }                                                                                        \
  } while (0)

void push_to_dblarray(DblArray *hm, char *key, double value);
uint32_t get_hashmap_location(const char* key, size_t capacity);
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);

#endif /* ifndef _SDM_LIB_H */

