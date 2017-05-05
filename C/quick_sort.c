#include <stdlib.h>
#include <stdio.h>
#include "quick_sort.h"

void partition_3_way(void** array, size_t start, size_t end, size_t pivot_pos, size_t* p1, size_t* p2,  KIComparator compare);
void quick_sort_3_way(void** array, size_t start, size_t end, KIComparator compare);
void quick_sort_3_way_g(void* array, size_t start, size_t end, size_t size, KIComparator compare);
void quick_sort_standard(void** array, size_t start, size_t end, KIComparator compare);

static size_t umin(size_t e1, size_t e2) {
  return e1 < e2 ? e1 : e2;
}

static size_t random_int(size_t size) {
  return (size_t) (drand48() * size);
}

static void swap(void** e1,void** e2) {
  void* tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}



static void array_block_swap_to_end(void** array, size_t start, size_t end, size_t len) {
  size_t b1_start = start;
  size_t b2_start = (size_t) (end - len + 1);
  for(size_t i=0; i<len; ++i) {
    swap(&array[b1_start+i], &array[b2_start+i]);
  }
}


// Partition the given array putting all pivot elements in the center of it.
// After the call *p1 will be the index of the last array cell whose content
// is less than the pivot, and *p2 will be the first one larger than the pivot.
//
// The function is based on the following invariant:
// i is the last index of elements less than the pivot
// p is the first index of elements equal to the pivot
// all indices between i and p contain elements larger than the pivot
//
// Given this invariant, at the end of the work all pivots will be accumulated
// at the end of the array. Last step in the procedure swap those values
// with the first elements larger than the pivot.

void partition_3_way(void** array, size_t start, size_t end, size_t pivot_pos, size_t* p1, size_t* p2,  KIComparator compare) {
  swap(&array[end], &array[pivot_pos]);
  const void* pivot = array[end];

  size_t i = (size_t)(start-1);
  size_t p = end;
  for(size_t j=start; j<p;) {
    if(compare(array[j], pivot)==0) {
      p-=1;
      swap(&array[j], &array[p]);
      continue;
    }

    if(compare(array[j], pivot) < 0) {
      ++i;
      swap(&array[i], &array[j]);
    }

    ++j;
  }

  size_t l = umin(p-1-i, end-p+1);
  array_block_swap_to_end(array, i+1, end, l);

  if(i==(size_t)-1) {
    *p1 = 0;
  } else {
    *p1 = i;
  }

  if(l == p-1-i) {
    *p2 = end-l+1;
  } else {
    *p2 = i+l+1;
  }
}



// int partition(const void** array, size_t start, size_t end, KIComparator compare) {
//   int pivot_pos = start + random_int(end-start);
//   const void* pivot = array[end];
//   swap( &array[end], &array[pivot_pos]);
//
//   int i = start;
//   for(int j=start; j <= end - 1; ++j) {
//     if(compare(array[j], pivot) < 0) {
//       swap(&array[i], &array[j]);
//       ++i;
//     }
//   }
//
//   swap(&array[i], &array[end]);
//   return i;
// }

static size_t partition(void** array, size_t first,size_t last, KIComparator compare){
  swap(&array[first], &array[first + random_int(last-first)]);
  const void* pivot = array[first];
  size_t i = first+1, j = last;
  while(i<=j){
    if(compare(array[i], pivot)<0)
      i++;
    else if(compare(array[j],pivot)>0)
      j--;
    else{
      swap(&array[i], &array[j]);
      i++;
      j--;
    }
  }
  swap(&array[first], &array[j]);
  return j;
}

void quick_sort_standard(void** array, size_t start, size_t end, KIComparator compare) {
  if( end <= start ) {
    return;
  }

  size_t pivot_pos = partition(array, start, end, compare);

  quick_sort_standard(array, start, pivot_pos, compare);
  quick_sort_standard(array, pivot_pos+1, end, compare );
}

void quick_sort_3_way(void** array, size_t start, size_t end, KIComparator compare) {
  if(end <= start ) {
    return;
  }

  size_t pivot_pos = start + random_int(end-start);
  size_t p1, p2;
  partition_3_way(array, start, end, pivot_pos, &p1, &p2, compare);

  quick_sort_3_way(array, start, p1, compare);
  quick_sort_3_way(array, p2, end, compare);
}



void quick_sort(void** array, size_t count, KIComparator compare) {
  if(count == 0) {
    return;
  }
  quick_sort_standard(array, 0, count-1, compare);
  // quick_sort_3_way(array, 0, count-1, compare);
}
