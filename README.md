# SDM C libraries

When writing C, I find myself reimplementing the same things over and over again.  Reading entire files, string-views, dynamic arrays, etc.

The intention of this library is to combine all of these things into a single library.  In the future I might smoosh it into a single header file, turning it into an STB style thing.  Not sure yet.

## Quick start

The following will compile and run the `test.c` file, which shows off (and tests) most of the functionality.

```console
$ make run
```
# Provided functionality

This library provides the following:

- `char *sdm_shift_args(int *argc, char ***argv);`      Peel arguments off the **argv array typically provided to main, decrementing argc appropriately.
- `char *sdm_read_entire_file(const char *file_path);`  Read the contents of a file into a character array. This character array is malloc'ed and so should be freed by the user.
- `SDM_FREE_AND_NULL(ptr)`                              Free the memory pointed to by ptr, and then set ptr to NULL.
- `#define SDM_FREE SDM_FREE_AND_NULL`
- `#define SDM_MALLOC malloc`

## DYNAMIC ARRAYS

- `SDM_ENSURE_ARRAY_CAP(da, cap)`       Ensure that the dynamic array, da, has a capacity equal-to cap. Realloc is used if needed.
- `SDM_ENSURE_ARRAY_MIN_CAP(da, cap)`   Ensure that the dynamic array, da, has a capacity equal-to or greater-than cap. Realloc is used if needed.
- `DEFAULT_CAPACITY 128`                Default capacity in bytes to be used when the capacity has not been set by the user.
- `SDM_ARRAY_PUSH(da, item)`            Push the value of item to the dynamic array, da, reallocing if needed.
- `SDM_ARRAY_FREE(da)`                  Free the memory in the dynamic array, da, and zero things
- `SDM_ARRAY_RESET(da)`                 Reset the length of the dynamic array, da, to zero, effectively emptying it. No memory is freed by this.

## STRING VIEWS

- `sdm_string_view sdm_cstr_as_sv(char *cstr);`                                 Return a string view containing the provided c-string
- `sdm_string_view sdm_sized_str_as_sv(char *cstr, size_t length);`             Return a string view containing the c-string with the provided size.
- `sdm_string_view sdm_sv_pop_by_delim(sdm_string_view *SV, const char delim);` Pop off the start of the provided string view up to the given delimiter.
- `void sdm_sv_trim(sdm_string_view *SV);`                                      Trim any whitespace from the start of the string view
- `SDM_SV_F "%.*s"`                                                             A printf helper.
- `SDM_SV_Vals(S) (int)(S).length, (S).data`                                    A printf helper.

## MEMORY ARENA

- `#define SDM_ARENA_DEFAULT_CAP 256 * 1024*1024`              Default capacity of the memory arena when not supplied by the user
- `void sdm_arena_init(sdm_arena_t *arena, size_t capacity);`  Initialise a memory arena with a certain capacity and malloc the required space.
- `void *sdm_arena_alloc(sdm_arena_t *arena, size_t size);`    Allocate a region of size bytes in the given arena, and return a pointer to the start of this region.
- `void sdm_arena_free(sdm_arena_t *arena);`                   Deallocate all memory in the arena, and zero everything

