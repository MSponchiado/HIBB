#ifndef __HEAP_H__
#define __HEAP_H__

#include "inter.h"

// typedef struct {
//     long int capacity;
//     long int size;
//     int ndim;
//     double *point;
//     t_indice **array;
//     double *dimsize;
// } t_heap;

typedef struct {
    long int capacity;
    long int size;
    double tolerance;
    t_indice **array;
} t_heap;

void heap_init(t_heap *h);
void heap_update(t_heap *h, double tol);
t_indice* heap_getmin(t_heap *h);
t_indice* heap_extractmin(t_heap *h);

//void heap_init(t_heap *h, double *dsize, int n);
//void heap_update(t_heap *h, double *p);
int heap_insert(t_heap *h, t_indice *box);
//t_indice* heap_getmax(t_heap *h);
//t_indice* heap_extractmax(t_heap *h);
long int heap_size(t_heap *h);
void heap_free(t_heap *h);

int heap_increase(t_heap *h);
int heap_decrease(t_heap *h);
void heap_up(t_heap *h, long int pos);
void heap_down(t_heap *h, long int pos);
void heap_heapify(t_heap *h);

#endif
