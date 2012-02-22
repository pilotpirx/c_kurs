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
    int i;
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


Heap* create_heap(int size)
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


void destroy_heap(Heap *heap)
{
    if (heap->nodes) {
        free(heap->nodes);
    }

    if (heap) {
        free(heap);
    }
}


void* find_min(Heap *heap)
{
    if (heap->size == 0) {
        return NULL;
    }

    return heap->nodes[0].data;
}


void delete_min(Heap *heap)
{
    if (heap->size == 0) {
        return;
    }

    heap->nodes[0] = heap->nodes[heap->size - 1];

    heap->size -= 1;

    heapify_down(heap);
}


int insert(Heap *heap, double key, void *data)
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


void print_heap(Heap *heap)
{
    int i, j, h, w, t;
    int step_size;
    int height = ceil(log2((double) heap->size + 1));
    int width = 4 * pow(2, height);

    char **out = malloc(height * sizeof(char*));

    for (i = 0; i < height; i++) {
        out[i] = malloc((width + 1) * sizeof(char));
        for (w = 0; w < width; w++) {
            out[i][w] = ' ';
        }
        out[i][width] = '\0';
    }

    i = 0;
    step_size = width;
    for (h = 0; h < height; h++) {
        for (w = 0; w < width / step_size; w++) {
                for(t = 0; t < step_size / 2; t++) {
                    printf(" ");
                }
                printf("%1.0f", heap->nodes[i].key);
                for(t = 0; t < step_size / 2 - 1; t++) {
                    printf(" ");
                }
                i++;
                if (i >= heap->size) {
                    break;
                }
        }
        printf("\n");
        step_size /= 2;
    }
}
