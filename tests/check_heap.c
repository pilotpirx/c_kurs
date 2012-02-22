#include "heap.h"
#include <stdio.h>
#include <check.h>

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
        heap_insert(heap, keys[i], (void*) i); /* misuse of the data field */
    }

    fail_unless(heap->size == 7);
    fail_unless((int) heap_find_min(heap) == 4);

}
END_TEST


Suite *
money_suite (void)
{
  Suite *s = suite_create ("heap");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");

  tcase_add_test (tc_core, test_heap_create);
  tcase_add_test (tc_core, test_heap_grow);
  tcase_add_test (tc_core, test_heap_insert);

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
