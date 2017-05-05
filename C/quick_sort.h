#pragma once

typedef int (*KIComparator)(const void*, const void*);

void quick_sort(void** array, size_t count, KIComparator compare);
