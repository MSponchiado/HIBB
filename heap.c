#include <stdlib.h>
#include <stdio.h>
#include "inter.h"
#include "constantes.h"
#include "heap.h"

#define HEAP_INC_CAPACITY 4096

#define HEAP_PARENT(i) ((i-1)/2)
#define HEAP_LEFT(i)   ((i)*2+1)
#define HEAP_RIGHT(i)  ((i)*2+2)

int heap_priority(t_heap *h, t_indice *a, t_indice *b) {
    if (PRIORITY_BY_STAGNATION) {
        if (a->cestag < b->cestag)
            return 1;
        if (a->cestag > b->cestag)
            return 0;
    }
    if (((a->viol <= h->tolerance) && (b->viol <= h->tolerance))
        || (a->viol == b->viol))
        return a->cost < b->cost;
    else if (a->viol < b->viol)
        return 1;
    else
        return 0;
}

int heap_resize(t_array *a, long int newcapacity) {
    t_indice **newarray = realloc(a->array, sizeof(t_indice *) * newcapacity);
    if (!newarray) {
        printf("memory allocation error in heap_resize\n");
        return 0;
    }
    a->capacity = newcapacity;
    a->array = newarray;
    return 1;
}

int heap_increase(t_array *a) {
    return heap_resize(a, a->capacity + HEAP_INC_CAPACITY);
}

int heap_decrease(t_array *a) {
    return heap_resize(a, a->capacity - HEAP_INC_CAPACITY);
}

void heap_up(t_heap *h, long int pos) {
    long int parent;
    t_indice *tmp;

    while (pos > 0) {
        parent = HEAP_PARENT(pos);
        // h->storage.array[parent] has priority over h->storage.array[pos]
        if (heap_priority(h, h->storage.array[parent], h->storage.array[pos]))
            break;
        tmp = h->storage.array[parent];
        h->storage.array[parent] = h->storage.array[pos];
        h->storage.array[pos] = tmp;
        pos = parent;
    }
}

void heap_down(t_heap *h, long int pos) {
    long int minchild, rch;
    t_indice *tmp;

    while ((minchild = HEAP_LEFT(pos)) < h->storage.size) {
        if (((rch = HEAP_RIGHT(pos)) < h->storage.size) &&
            // h->storage.array[rch] has priority over h->storage.array[minchild]
            heap_priority(h, h->storage.array[rch], h->storage.array[minchild]))
            minchild = rch;
        // h->storage.array[pos] has priority over h->storage.array[minchild]
        if (heap_priority(h, h->storage.array[pos], h->storage.array[minchild]))
            break;
        tmp = h->storage.array[minchild];
        h->storage.array[minchild] = h->storage.array[pos];
        h->storage.array[pos] = tmp;
        pos = minchild;
    }
}

void heap_heapify(t_heap *h) {
    for (long int i = h->storage.size / 2 - 1; i >= 0; i--) {
        heap_down(h, i);
    }
}

void heap_init(t_heap *h) {
    h->tolerance = 0.0;
    h->frozen = 0;
    h->storage.capacity = 0;
    h->storage.size = 0;
    h->storage.array = NULL;
    h->freezer.capacity = 0;
    h->freezer.size = 0;
    h->freezer.array = NULL;
}

void heap_free(t_heap *h) {
    free(h->storage.array);
    free(h->freezer.array);
    heap_init(h);
}

void heap_freeze(t_heap *h) {
    h->frozen = 1;
}

void heap_unfreeze(t_heap *h) {
    h->frozen = 0;
    for (long int i = 0; i < h->freezer.size; i++) {
        heap_insert(h, h->freezer.array[i]);
    }
    h->freezer.size = 0;
    if (h->freezer.capacity > HEAP_INC_CAPACITY) {
        heap_resize(&(h->freezer), HEAP_INC_CAPACITY);
    }
}

long int heap_size(t_heap *h) {
    return h->storage.size + h->freezer.size;
}

void heap_update(t_heap *h, double tol) {
    h->tolerance = tol;
    heap_heapify(h);
}

t_indice* heap_get(t_heap *h) {
    if (!h->storage.size && h->frozen)
        heap_unfreeze(h);
    if (!h->storage.size)
        return NULL;
    return h->storage.array[0];
}

t_indice* heap_extract(t_heap *h) {
    t_indice *box;

    if (!h->storage.size && h->frozen)
        heap_unfreeze(h);
    if (!h->storage.size)
        return NULL;
    box = h->storage.array[0];
    h->storage.size--;
    h->storage.array[0] = h->storage.array[h->storage.size];
    heap_down(h, 0);
    if ((h->storage.capacity - h->storage.size) > HEAP_INC_CAPACITY) {
        heap_decrease(&(h->storage));
    }
    return box;
}

int heap_insert(t_heap *h, t_indice *box) {
    long int pos;

    if (h->frozen) {
        if (h->freezer.size == h->freezer.capacity)
            heap_increase(&(h->freezer));
        pos = h->freezer.size;
        h->freezer.array[pos] = box;
        h->freezer.size++;
    }
    else {
        if (h->storage.size == h->storage.capacity)
            heap_increase(&(h->storage));
        pos = h->storage.size;
        h->storage.array[pos] = box;
        h->storage.size++;
        heap_up(h, pos);
    }
    return 1;
}
