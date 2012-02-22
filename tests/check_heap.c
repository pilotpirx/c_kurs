#include <stdio.h>
#include <check.h>
#include <stdlib.h>

#include "heap.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


START_TEST (test_heap_create)
{
    Heap *heap = heap_create(50);

    fail_unless(heap->size_allocated == 50);
    fail_unless(heap->size == 0);
    fail_unless(heap->nodes != NULL);

    heap_destroy(heap);
}
END_TEST


START_TEST (test_heap_grow)
{
    int i;
    double data = 5;

    Heap *heap = heap_create(0);

    for (i = 0; i < 50; i++) {
        heap_insert(heap, i, &data);
    }

    fail_unless(heap->size == 50);
    fail_unless(heap->size_allocated >= 50);
    fail_unless(heap->size_allocated <= 1000);

    heap_destroy(heap);
}
END_TEST


START_TEST (test_heap_insert)
{
    double data = 5;

    Heap *heap = heap_create(0);

    heap_insert(heap, 5, &data);
    fail_unless(heap->nodes[0].key == 5);
    fail_unless(heap->size == 1);

    heap_insert(heap, 4, &data);
    fail_unless(heap->nodes[0].key == 4);
    fail_unless(heap->nodes[1].key == 5);
    fail_unless(heap->size == 2);

    heap_insert(heap, 1, &data);
    fail_unless(heap->nodes[0].key == 1);
    fail_unless(heap->nodes[1].key == 5);
    fail_unless(heap->nodes[2].key == 4);

    heap_insert(heap, -1, &data);
    fail_unless(heap->nodes[0].key == -1);
    fail_unless(heap->nodes[1].key == 1);
    fail_unless(heap->nodes[2].key == 4);
    fail_unless(heap->nodes[3].key == 5);

    heap_destroy(heap);

    heap = heap_create(0);
    double keys[7] = {0.1, -1.5, 5, 100, -1.6, 0, 5};
    int i;
    for (i = 0; i < 7; i++) {
        heap_insert(heap, keys[i], keys + i);
    }

    fail_unless(heap->size == 7);
    fail_unless(*(double *) heap_find_min(heap) == -1.6);

}
END_TEST


START_TEST (test_heap_find_min)
{
    Heap *heap = heap_create(0);
    long int data[50];
    long int min = RAND_MAX;
    int i;

    srandom(42);

    for (i = 0; i < 50; i++) {
        data[i] = random();
        if (data[i] < min) {
            min = data[i];
        }
    }

    for (i = 0; i < 50; i++) {
        heap_insert(heap, data[i], data + i);
    }

    fail_unless(*(double *)heap_find_min(heap) != min);
}
END_TEST


int num_cmp(const void *arg1, const void *arg2)
{
    long int a = * (long int *) arg1;
    long int b = * (long int *) arg2;
    if (a == b) {
        return 0;
    }
    if (a > b) {
        return 1;
    }
    return -1;
}

#define N 100

START_TEST (test_heap_delete_min)
{
    Heap *heap = heap_create(0);
    long int data[N];
    long int data_copy[N];
    long int tmp;
    int i;

    srandom(84);

    for (i = 0; i < N; i++) {
        data[i] = random();
        data_copy[i] = data[i];
    }

    for (i = 0; i < N; i++) {
        heap_insert(heap, data[i], data + i);
    }

    qsort(data_copy, N, sizeof(long int), num_cmp);

    for (i = 0; i < N; i++) {
        tmp = * (long int *) heap_find_min(heap);
        fail_unless(tmp == data_copy[i]);
        heap_delete_min(heap);
    }
}
END_TEST


Suite * money_suite (void)
{
    Suite *s = suite_create ("heap");

    /* Core test case */
    TCase *tc_core = tcase_create ("Core");

    tcase_add_test (tc_core, test_heap_create);
    tcase_add_test (tc_core, test_heap_grow);
    tcase_add_test (tc_core, test_heap_insert);
    tcase_add_test (tc_core, test_heap_find_min);
    tcase_add_test (tc_core, test_heap_delete_min);

    suite_add_tcase (s, tc_core);

    return s;
}


int main (void)
{
    int number_failed;
    Suite *s = money_suite ();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
