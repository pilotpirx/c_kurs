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

Heap* create_heap(int size);

void destroy_heap(Heap *heap);

void* find_min(Heap *heap);

void delete_min(Heap *heap);

int insert(Heap *heap, double key, void *data);

void print_heap(Heap *heap);

#endif
