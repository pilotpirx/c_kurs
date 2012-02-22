#include "heap.h"
#include <malloc.h>
#include <math.h>

#define MIN(a, b) ((a) > (b)? (b) : (a))

int grow_heap(Heap *heap)
{
    HeapNode *new_ptr;
    new_ptr = realloc(heap->nodes, 2 * (heap->size + 1) * sizeof(HeapNode));
    if (!new_ptr) {
        return 1;
    }
    heap->nodes = new_ptr;
    heap->size_allocated = 2 * (heap->size + 1);

    return 0;
}


void heap_swap(Heap *heap, int i, int j)
{
    HeapNode tmp;
    tmp = heap->nodes[i];
    heap->nodes[i] = heap->nodes[j];
    heap->nodes[j] = tmp;
}


void heapify_up(Heap *heap)
{
    int i;
    HeapNode *nodes = heap->nodes;

    i = heap->size - 1;
    while (i > 0) {
        if (nodes[i].key >= nodes[(i - 1) / 2].key) {
            break;
        }
        heap_swap(heap, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}


void heapify_down(Heap *heap)
{
    unsigned int i;
    HeapNode *nodes = heap->nodes;
    double key_right;
    double key_left;
    double key;

    i = 0;
    while (1) {
        if (2 * i >= heap->size) {
            break;
        }

        if (2 * i + 1 >= heap->size) {
            key_left = nodes[2 * i].key;
            if (key > key_left) {
                heap_swap(heap, i, 2 * i);
                i = 2 * i;
                break;
            }
        }

        key_right = nodes[2 * i + 1].key;
        key_left = nodes[2 * i].key;
        key = nodes[i].key;

        if (key <= MIN(key_right, key_left)) {
            break;
        }

        if (key_right <= key_left) {
            heap_swap(heap, i, 2 * i + 1);
            i = 2 * i + 1;
        }
        else {
            heap_swap(heap, i, 2 * i);
            i = 2 * i;
        }
    }
}


Heap* heap_create(int size)
{
    Heap *heap = malloc(sizeof(Heap));

    if (!heap) {
        return NULL;
    }

    heap->nodes = malloc( size * sizeof(HeapNode));

    if (!heap->nodes) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->size_allocated = size;

    return heap;
}


void heap_destroy(Heap *heap)
{
    if (heap->nodes) {
        free(heap->nodes);
    }

    if (heap) {
        free(heap);
    }
}


void* heap_find_min(Heap *heap)
{
    if (heap->size == 0) {
        return NULL;
    }

    return heap->nodes[0].data;
}


void heap_delete_min(Heap *heap)
{
    if (heap->size == 0) {
        return;
    }

    heap->nodes[0] = heap->nodes[heap->size - 1];

    heap->size -= 1;

    heapify_down(heap);
}


int heap_insert(Heap *heap, double key, void *data)
{
    int failure = 0;

    if (heap->size_allocated == heap->size) {
        failure = grow_heap(heap);
    }

    heap->nodes[heap->size].key = key;
    heap->nodes[heap->size].data = data;

    heap->size += 1;

    heapify_up(heap);

    return failure;
}


void heap_print(Heap *heap)
{
    unsigned int i, h, w, t;
    unsigned int step_size;
    unsigned int height = ceil(log2((double) heap->size + 1));
    unsigned int width = 4 * pow(2, height);

    i = 0;
    step_size = width;
    for (h = 0; h < height; h++) {
        for (w = 0; w < width / step_size; w++) {
                for(t = 0; t < step_size / 2; t++) {
                    printf(" ");
                }
                printf("%1.0f", heap->nodes[i].key);
                i++;
                for(t = 0; t < step_size / 2 - 1; t++) {
                    printf(" ");
                }
                if (i >= heap->size) {
                    break;
                }
        }
        printf("\n");
        step_size /= 2;
    }
}
