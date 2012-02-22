#ifndef HEAP_H
#define HEAP_H

typedef struct {
    double key;
    void *data;
} HeapNode;

typedef struct {
    unsigned int size;
    unsigned int size_allocated;
    HeapNode *nodes;
} Heap;

Heap* heap_create(int size);

void heap_destroy(Heap *heap);

void* heap_find_min(Heap *heap);

void heap_delete_min(Heap *heap);

int heap_insert(Heap *heap, double key, void *data);

void heap_print(Heap *heap);

#endif
