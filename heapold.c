#include <stdlib.h>
#include <stdio.h>
#include "inter.h"
#include "heap.h"

#define HEAP_INC_CAPACITY 4096

#define HEAP_PARENT(i) ((i-1)/2)
#define HEAP_LEFT(i)   ((i)*2+1)
#define HEAP_RIGHT(i)  ((i)*2+2)

// void heap_init(t_heap *h, double *dsize, int n) {
//     h->array = NULL;
//     h->point = NULL;
//     h->capacity = 0;
//     h->size = 0;
//     h->ndim = n;
//     h->dimsize = dsize;
// }
//
// void heap_free(t_heap *h) {
//     free(h->array);
//     heap_init(h, NULL, h->ndim);
// }

void heap_init(t_heap *h) {
    h->array = NULL;
    h->capacity = 0;
    h->size = 0;
    h->tolerance = 0.0;
}

void heap_free(t_heap *h) {
    free(h->array);
    heap_init(h);
}

// void heap_distance(t_heap *h, t_indice *box) {
//     double di, ds;
//     box->dist = 0.0;
//     for (int i = 0; i < h->ndim; i++) {
//         di = box->ind[i].bi - h->point[i];
//         ds = h->point[i] - box->ind[i].bs;
//         if (di < 0.0)
//             di = 0.0;
//         if (ds < 0.0)
//             ds = 0.0;
//         box->dist += (di + ds) / h->dimsize[i];
//     }
// }

// cada vez que tiver um novo bp, chama
// void heap_update(t_heap *h, double *p) {
//     h->point = p;
//     for (long int i = 0; i < h->size; i++)
//         heap_distance(h, h->array[i]);
//     heap_heapify(h);
// }

void heap_update(t_heap *h, double tol) {
    h->tolerance = tol;
    heap_heapify(h);
}

int heap_cmpless(t_heap *h, t_indice *a, t_indice *b) {
    if (((a->viol <= h->tolerance) && (b->viol <= h->tolerance))
        || (a->viol == b->viol))
        return a->cost < b->cost;
    else if (a->viol < b->viol)
        return 1;
    else
        return 0;
}


long int heap_size(t_heap *h) {
    return h->size;
}

// retorna a cabeça da fila, mas não extrai
// t_indice* heap_getmax(t_heap *h) {
//     if (!h->size)
//         return NULL;
//     return h->array[0];
// }

t_indice* heap_getmin(t_heap *h) {
    if (!h->size)
        return NULL;
    return h->array[0];
}


int heap_resize(t_heap *h, long int newcapacity) {
    t_indice **newarray = realloc(h->array, sizeof(t_indice *) * newcapacity);
    if (!newarray) {
        printf("memory allocation error in heap_resize\n");
        return 0;
    }
    h->capacity = newcapacity;
    h->array = newarray;
    return 1;
}

int heap_increase(t_heap *h) {
    return heap_resize(h, h->capacity + HEAP_INC_CAPACITY);
}

int heap_decrease(t_heap *h) {
    return heap_resize(h, h->capacity - HEAP_INC_CAPACITY);
}

// void heap_up(t_heap *h, long int pos) {
//     long int parent;
//     t_indice *tmp;
//
//     while (pos > 0) {
//         parent = HEAP_PARENT(pos);
//         // h->array[parent] > h->array[pos]
//         if (h->array[parent]->dist > h->array[pos]->dist)
//             break;
//         tmp = h->array[parent];
//         h->array[parent] = h->array[pos];
//         h->array[pos] = tmp;
//         pos = parent;
//     }
// }
//
// void heap_down(t_heap *h, long int pos) {
//     long int maxchild, rch;
//     t_indice *tmp;
//
//     while ((maxchild = HEAP_LEFT(pos)) < h->size) {
//         if (((rch = HEAP_RIGHT(pos)) < h->size) &&
//             // h->array[rch] > h->array[maxchild]
//             (h->array[rch]->dist > h->array[maxchild]->dist))
//             maxchild = rch;
//         // h->array[pos] > h->array[maxchild]
//         if (h->array[pos]->dist > h->array[maxchild]->dist)
//             break;
//         tmp = h->array[maxchild];
//         h->array[maxchild] = h->array[pos];
//         h->array[pos] = tmp;
//         pos = maxchild;
//     }
// }
//
// // retorna a cabeça da fila e remove
// t_indice* heap_extractmax(t_heap *h) {
//     t_indice *box;
//
//     if (!h->size)
//         return NULL;
//     if ((h->capacity - h->size) > HEAP_INC_CAPACITY) {
//         heap_decrease(h);
//     }
//     box = h->array[0];
//     h->size--;
//     h->array[0] = h->array[h->size];
//     heap_down(h, 0);
//     return box;
// }
//
// // insere uma caixa na fila
// int heap_insert(t_heap *h, t_indice *box) {
//     long int pos;
//
//     if (h->size == h->capacity)
//         heap_increase(h);
//     heap_distance(h, box);
//     pos = h->size;
//     h->array[pos] = box;
//     h->size++;
//     heap_up(h, pos);
//     return 1;
// }

void heap_up(t_heap *h, long int pos) {
    long int parent;
    t_indice *tmp;

    while (pos > 0) {
        parent = HEAP_PARENT(pos);
        // h->array[parent] < h->array[pos]
        if (heap_cmpless(h, h->array[parent], h->array[pos]))
            break;
        tmp = h->array[parent];
        h->array[parent] = h->array[pos];
        h->array[pos] = tmp;
        pos = parent;
    }
}

void heap_down(t_heap *h, long int pos) {
    long int minchild, rch;
    t_indice *tmp;

    while ((minchild = HEAP_LEFT(pos)) < h->size) {
        if (((rch = HEAP_RIGHT(pos)) < h->size) &&
            // h->array[rch] < h->array[minchild]
            heap_cmpless(h, h->array[rch], h->array[minchild]))
            minchild = rch;
        // h->array[pos] < h->array[minchild]
        if (heap_cmpless(h, h->array[pos], h->array[minchild]))
            break;
        tmp = h->array[minchild];
        h->array[minchild] = h->array[pos];
        h->array[pos] = tmp;
        pos = minchild;
    }
}

t_indice* heap_extractmin(t_heap *h) {
    t_indice *box;

    if (!h->size)
        return NULL;
    if ((h->capacity - h->size) > HEAP_INC_CAPACITY) {
        heap_decrease(h);
    }
    box = h->array[0];
    h->size--;
    h->array[0] = h->array[h->size];
    heap_down(h, 0);
    return box;
}

int heap_insert(t_heap *h, t_indice *box) {
    long int pos;

    if (h->size == h->capacity)
        heap_increase(h);
    pos = h->size;
    h->array[pos] = box;
    h->size++;
    heap_up(h, pos);
    return 1;
}

void heap_heapify(t_heap *h) {
    for (long int i = h->size / 2 - 1; i >= 0; i--) {
        heap_down(h, i);
    }
}
