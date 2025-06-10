#ifndef __HEAP_H__
#define __HEAP_H__

#include "inter.h"

typedef struct {
    long int capacity;
    long int size;
    t_indice **array;
} t_array;

typedef struct {
    double tolerance;
    int frozen;
    t_array storage;
    t_array freezer;
} t_heap;

void heap_init(t_heap *h);
void heap_free(t_heap *h);
void heap_freeze(t_heap *h);
void heap_unfreeze(t_heap *h);
long int heap_size(t_heap *h);
void heap_update(t_heap *h, double tol);
t_indice* heap_get(t_heap *h);
t_indice* heap_extract(t_heap *h);
int heap_insert(t_heap *h, t_indice *box);

#endif
