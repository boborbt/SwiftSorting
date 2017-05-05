#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "quick_sort.h"
#include "dataset.h"
#include "errors.h"
#include "mem.h"


static void exec_and_print_with_dup_storage(Dataset* dataset, void (^callback)(Array*)) {
  Array* array = Array_dup(Dataset_get_storage(dataset));
  callback(array);
  Dataset_print_storage(array, 10);
  Array_free(array);
}

void PrintTime_print(char* msg, void (^callback)(void)) {
  clock_t start = clock();
  callback();
  clock_t end = clock();

  printf("Elapsed time:%lf\n", (end - start) / (double)CLOCKS_PER_SEC);
}

// qsort passes to the comparison function a pointer to the array element,
// not the element itself. Dataset_compare_field? assumes that the given
// elements are actually the array elements themeselves. We need to dereference
// the pointers to make the things agree.
static int qsort_compare_field1(const void* e1, const void* e2) {
  return Dataset_compare_field1( *( void* const*)e1, *(void* const*)e2 );
}

static int qsort_compare_field2(const void* e1, const void* e2) {
  return Dataset_compare_field2( *(void* const*)e1, *(void* const*)e2 );
}

static int qsort_compare_field3(const void* e1, const void* e2) {
  return Dataset_compare_field3( *(void* const*)e1, *(void* const*)e2 );
}


static void test_qsort(Dataset* input_dataset) {
  // exec_and_print_with_dup_storage(input_dataset, ^(Array* dataset) {
  //   PrintTime_print("field1", ^{
  //     printf("Sorting according to field1\n");
  //     qsort((void**) Array_carray(dataset), Array_size(dataset), sizeof(Record*), qsort_compare_field1);
  //     printf("Done!\n");
  //   });
  // });

  exec_and_print_with_dup_storage(input_dataset, ^(Array* dataset) {
    PrintTime_print("field2", ^{
      printf("Sorting according to field2\n");
      qsort((void**) Array_carray(dataset), Array_size(dataset), sizeof(Record*), qsort_compare_field2);
      printf("Done!\n");
    });
  });

  // exec_and_print_with_dup_storage(input_dataset, ^(Array* dataset) {
  //   PrintTime_print(pt, "field3", ^{
  //     printf("Sorting according to field3\n");
  //     qsort((void**) Array_carray(dataset), Array_size(dataset), sizeof(Record*), qsort_compare_field3);
  //     printf("Done!\n");
  //   });
  // });

}


static void test_algorithm(Dataset* input_dataset, void (*sort)(void**, size_t, int(*)(const void*, const void*))) {
  // exec_and_print_with_dup_storage(input_dataset, ^(Array* dataset) {
  //   PrintTime_print("field1", ^{
  //     printf("Sorting according to field1\n");
  //     sort((void**)Array_carray(dataset), Array_size(dataset), Dataset_compare_field1);
  //     printf("Done!\n");
  //   });
  // });
  exec_and_print_with_dup_storage(input_dataset, ^(Array* dataset) {
    PrintTime_print("field2", ^{
      printf("Sorting according to field2\n");
      sort((void**)Array_carray(dataset), Array_size(dataset), Dataset_compare_field2);
      printf("Done!\n");
    });
  });
  // exec_and_print_with_dup_storage(input_dataset, ^(Array* dataset) {
  //   PrintTime_print("field3", ^{
  //     printf("Sorting according to field3\n");
  //     sort((void**)Array_carray(dataset), Array_size(dataset), Dataset_compare_field3);
  //     printf("Done!\n");
  //   });
  // });
}


static void print_usage() {
  printf("Usage: measure_time <opt> <file name>\n");
  printf(" opts: -q use quick_sort algorithm\n");
  printf("       -Q use stdlib qsort algorithm\n");
  printf("       -h print this message\n");
}

static int char_included(char ch, char chars[], size_t size) {
  for(size_t i=0; i<size; ++i) {
    if(ch == chars[i]) return 1;
  }

  return 0;
}

static void check_arguments(int argc, char** argv) {
  if(argc <= 2) {
    printf("Expected more than 2 arguments, got %d\n", argc);
    print_usage();
    exit(ERROR_ARGUMENT_PARSING);
  }

  if(strlen(argv[1])!=2 || argv[1][0] != '-' || !char_included(argv[1][1], (char[]){'q','h', 'Q'}, 3)) {
    printf("Option %s not recognized\n", argv[1]);
    print_usage();
    exit(ERROR_ARGUMENT_PARSING);
  }

  if(!strcmp(argv[1], "-h")) {
    print_usage();
    exit(0);
  }
}


int main(int argc, char* argv[]) {
  check_arguments(argc, argv);

  __block Dataset* dataset;
  PrintTime_print("Dataset_load", ^{
    printf("Loading dataset...\n");
    dataset = Dataset_load(argv[2]);
    printf("Done!\n");
  });

  Dataset_print(dataset, 10);

  switch(argv[1][1]) {
    case 'q':
      test_algorithm(dataset, quick_sort);
      break;
    case 'Q':
      test_qsort(dataset);
      break;
    default:
      assert(0); // should never get here
  }

  PrintTime_print("Dataset_free", ^{
    printf("Freeing dataset\n");
    Dataset_free(dataset);
    printf("Done!\n");
  });

  Mem_check_and_report();

  return 0;
}
