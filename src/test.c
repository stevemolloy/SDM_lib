#include <stdio.h>
#include <stdlib.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

char* random_words[256] = {
    "ABSTRACT",  "ACCIDENT",  "ACCOUNT",  "ACTION",  "ADVANTAGE",  "ADVENTURE",  "AFFECTION",  
    "AFTERNOON",  "AIRPLANE",  "ALCHEMY",  "ALIEN",  "ALPHABET",  "AMBITION",  "ANIMAL",  
    "ANTENNA",  "APPLE",  "ARROW",  "ASSEMBLY",  "ATTEMPT",  "AURORA",  "AUTUMN",  
    "BALCONY",  "BALLOON",  "BANANA",  "BARBECUE",  "BASKET",  "BATTLE",  "BEAUTY",  
    "BEDROOM",  "BEGINNING",  "BELIEF",  "BENEFIT",  "BICYCLE",  "BIOLOGY",  
    "BLIZZARD",  "BLOSSOM",  "BLUEBIRD",  "BOARDWALK",  "BOAT",  "BOLDNESS",  
    "BOULEVARD",  "BREAKFAST",  "BRIDGE",  "BROTHER",  "BUBBLE",  "BUCKET",  
    "BUILDING",  "BUTTERFLY",  "CABIN",  "CANDLE",  "CANYON",  "CARAVAN",  
    "CARPET",  "CARROT",  "CASTLE",  "CAVALRY",  "CELEBRATION",  "CELLAR",  
    "CEREMONY",  "CHALLENGE",  "CHAMPION",  "CHARISMA",  "CHEETAH",  "CHEMISTRY",  
    "CHESTNUT",  "CHIMNEY",  "CHOCOLATE",  "CIRCUS",  "CLARITY",  "CLASSROOM",  
    "CLEVERNESS",  "CLOUD",  "COASTLINE",  "COCOA",  "COFFEE",  "COMET",  
    "COMPASS",  "CONCERT",  "CONQUEST",  "CONSTELLATION",  "CONVERSATION",  "COURAGE",  
    "CRADLE",  "CRANE",  "CREATION",  "CRESCENT",  "CRYSTAL",  "CUPBOARD",  
    "CURTAIN",  "DAFFODIL",  "DAISY",  "DANCE",  "DAYDREAM",  "DECEMBER",  
    "DECISION",  "DEER",  "DELIGHT",  "DESERT",  "DESTINY",  "DIAMOND",  
    "DISCOVERY",  "DISTANCE",  "DOGWOOD",  "DOORWAY",  "DRAGONFLY",  "DREAMER",  
    "DROPLET",  "DUNE",  "EAGLE",  "EARTHQUAKE",  "ECSTASY",  "EFFORT",  
    "ELEMENT",  "EMBER",  "EMOTION",  "ENCHANTMENT",  "ENCOUNTER",  "ENERGY",  
    "ENIGMA",  "EQUATION",  "ESCAPE",  "EVENING",  "EVOLUTION",  "EXCELLENCE",  
    "EXPLORER",  "EXPRESSION",  "FACTORY",  "FAITHFULNESS",  "FALLING",  "FANTASY",  
    "FAREWELL",  "FEATHER",  "FICTION",  "FIELD",  "FIGURE",  "FILM",  
    "FIREPLACE",  "FLAMINGO",  "FLIGHT",  "FLOWER",  "FOREST",  "FOREVER",  
    "FORTUNE",  "FOUNTAIN",  "FREEDOM",  "FRIENDSHIP",  "FROST",  "GALAXY",  
    "GARDEN",  "GATEWAY",  "GENEROSITY",  "GENTLE",  "GESTURE",  "GIFTED",  
    "GLACIER",  "GLAMOUR",  "GLOWING",  "GOAL",  "GOODWILL",  "GRACEFUL",  
    "GRAZING",  "GRIFFIN",  "GUITAR",  "HARMONY",  "HARVEST",  "HAVEN",  
    "HEIGHTS",  "HEROISM",  "HIGHWAY",  "HILLTOP",  "HOLIDAY",  "HONEYBEE",  
    "HORIZON",  "HOUSE",  "HURRICANE",  "ILLUSION",  "IMAGINATION",  "INFINITY",  
    "INSIGHT",  "INSPIRATION",  "INTENTION",  "INVENTION",  "ISLAND",  "JASMINE",  
    "JELLYFISH",  "JOURNEY",  "JUBILEE",  "JUNGLE",  "JUSTICE",  "KINDNESS",  
    "KINGDOM",  "KITE",  "LADDER",  "LAGOON",  "LANTERN",  "LANDSCAPE",  
    "LANGUAGE",  "LAUGHTER",  "LAVENDER",  "LEGEND",  "LEMONADE",  "LIBERTY",  
    "LIGHTHOUSE",  "LILAC",  "LIMITLESS",  "LUCK",  "LULLABY",  "MAGAZINE",  
    "MAGICIAN",  "MAJESTIC",  "MEADOW",  "MEMORY",  "MERMAID",  "MIDNIGHT",  
    "MIRACLE",  "MISSION",  "MIST",  "MOLECULE",  "MONSOON",  "MOUNTAIN",  
    "MYSTERY",  "NARRATIVE",  "NECTAR",  "NIGHTFALL",  "NOONTIME",  "NOVELTY",  
    "NUTMEG",  "OASIS",  "OCEAN",  "OPERA",  "ORBIT",  "ORCHARD",  
    "OUTLOOK",  "OVERCOME",  "PACIFIC",  "PALACE",  "PANORAMA",  "PARADISE",  
    "PASSAGE",  "PASSION",  "PATIENCE",  "PEBBLE",  "PEGASUS",  "PERFECTION",  
    "PERSEVERANCE",  "PETAL",  "PHOENIX",  "PHRASE",  "PIONEER",  "PIRATE",  
    "PITCHER",  "PIXEL",  "PLANTATION",  "POETRY",  "PORTAL",  "POTENTIAL",  
};

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

static sdm_arena_t main_arena = {0};
static sdm_arena_t *active_arena = &main_arena;

void *active_alloc(size_t size) {
  return sdm_arena_alloc(active_arena, size);
}
#define ALLOC active_alloc

int main(int argc, char **argv) {
  sdm_shift_args(&argc, &argv);
  if (argc > 0) printf("Arguments:\n");
  while (argc > 0) {
    char *argument = sdm_shift_args(&argc, &argv);
    printf("\t%s\n", argument);
  }

  const char *file_path = "./src/test.c";
  char *contents = sdm_read_entire_file(file_path);
  free(contents);

  char *multi_line_str = "Hello\nWorld\nBlahblah\n";
  sdm_string_view multi_line_sv = sdm_cstr_as_sv(multi_line_str);
  sdm_string_view line_1 = sdm_sv_pop_by_delim(&multi_line_sv, '\n');
  sdm_string_view line_2 = sdm_sv_pop_by_delim(&multi_line_sv, '\n');
  printf("Line 1: "SDM_SV_F"\n", SDM_SV_Vals(line_1));
  printf("Line 2: "SDM_SV_F"\n", SDM_SV_Vals(line_2));

  char *test_trim = "   Hello!";
  sdm_string_view test_trim_sv = sdm_cstr_as_sv(test_trim);
  sdm_sv_trim(&test_trim_sv);
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
  for (size_t i=0; i<256; i++) {
    push_to_dblarray(&hashmap, random_words[i], i*1.001);
  }
  bool data_integrity_good = true;
  for (size_t i=0; i<256; i++) {
    int index = 0;
    GET_HASHMAP_INDEX(hashmap, random_words[i], &index);
    if (HM_VAL_AT(hashmap, index) != i*1.001) {
      fprintf(stderr, "Hashmap has value %f but should have %f\n", HM_VAL_AT(hashmap, index), i*1.001);
    }
    data_integrity_good = (index >= 0) && data_integrity_good && (HM_VAL_AT(hashmap, index) == i*1.001);
  }

  printf("The hashmap has a capacity of %zu\n", hashmap.capacity);
  if (data_integrity_good) {
    printf("Hashmap data is good\n");
  } else {
    fprintf(stderr, "Hashmap data is not good\n");
    return 1;
  }

  push_to_dblarray(&hashmap, "z", 3.14159);
  int index = 0;
  GET_HASHMAP_INDEX(hashmap, "z", &index);
  if (index<=0) {
    fprintf(stderr, "Index of \"x\" could not be found. This is an error.\n");
    return 1;
  }
  printf("Expecting 3.14159, got %f\n", HM_VAL_AT(hashmap, index));
  printf("The hashmap now has a capacity of %zu\n", hashmap.capacity);

  data_integrity_good = true;
  for (size_t i=0; i<256; i++) {
    int index = 0;
    GET_HASHMAP_INDEX(hashmap, random_words[i], &index);
    data_integrity_good = 
      (index >= 0) && 
      data_integrity_good && 
      (HM_VAL_AT(hashmap, index) == i*1.001);
  }

  if (data_integrity_good) {
    printf("Hashmap data is still good\n");
  } else {
    printf("Hashmap data has been corrupted\n");
    return 1;
  }

  FREE_HASHMAP(hashmap);

  DblArray new_hashmap = {0};
  char* my_key = "stephen";
  double my_value = 1.111;
  PUSH_TO_HASHMAP((&new_hashmap), my_key, my_value);
  GET_HASHMAP_INDEX(new_hashmap, my_key, &index);
  printf("Expecting \"%s\" to be equal to %f. Got %f\n", my_key, my_value, HM_VAL_AT(new_hashmap, index));

  FREE_HASHMAP(new_hashmap);

  printf("\nTesting the memory arena\n");
  size_t capacity = 4;   // Too small. Demonstrates the flexibility of the allocation strategy.
  sdm_arena_init(&main_arena, capacity);

  printf("Allocating 'original_string' on the stack in the usual way\n");
  char *original_string = "Hello world";

  printf("Allocating space in the arena for 'new_string'\n");
  char *new_string = ALLOC(strlen(original_string));

  memcpy(new_string, original_string, strlen(original_string));
  printf("Chaning the fifth character of 'new_string' to be a '#'\n");
  new_string[5] = '#';

  printf("original_string => %s\n", original_string);
  printf("new_string      => %s\n", new_string);

  size_t dbl_array_len = 20;
  printf("\nNow allocating space for %zu doubles in the same memory arena\n", dbl_array_len);
  double *dbl_array = ALLOC(dbl_array_len * sizeof(double));
  for (size_t i=0; i<dbl_array_len; i++) {
    dbl_array[i] = 1.0 / (i + 1);
  }
  for (size_t i=0; i<dbl_array_len; i++) {
   printf("dbl_array[%zu] = %f\n", i, dbl_array[i]);
  }

  printf("\n");
  printf("Clearing and deallocating the arena\n(Valgrind should be happy)\n");
  sdm_arena_free(&main_arena);

  return 0;
}
