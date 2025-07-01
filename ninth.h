#define _GNU_SOURCE
#include "defn.h"
#include <pthread.h>

typedef enum bool {
    False,
    True
} bool;

typedef struct Ninth {
    int values[3][3];
    bool possible[3][3][9];
    pthread_t thread;
} Ninth;
