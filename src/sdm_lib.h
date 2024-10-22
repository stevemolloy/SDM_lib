#ifndef _SDM_LIB_H
#define _SDM_LIB_H

#include <stdlib.h>

#define SDM_ARRAY_PUSH(da, item) do {                                   \
    if (((da).capacity == 0) || ((da).data == NULL)) {            \
      (da).capacity = DEFAULT_CAPACITY;                            \
      (da).data = calloc((da).capacity, sizeof((da).data[0]));   \
      if ((da).data == NULL) {                                     \
        fprintf(stderr, "Could not alloc memory. Quitting.\n");     \
        exit(1);                                                    \
      }                                                             \
    }                                                               \
    while ((da).length >= (da).capacity) {                        \
      (da).capacity *= 2;                                          \
      (da).data = realloc((da).data,                              \
                           (da).capacity * sizeof((da).data[0])); \
    }                                                               \
    (da).data[(da).length++] = item;                              \
  } while (0);

#define SDM_ARRAY_FREE(da) do { \
    free((da).data);        \
    (da).length = 0;        \
    (da).capacity = 0;      \
  } while (0);

#define SDM_ARRAY_RESET(da) do { \
    (da).length = 0;         \
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

#endif /* ifndef _SDM_LIB_H */

