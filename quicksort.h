// quicksort.h
#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include <omp.h>

#define DEBUG_MODE 0

#define DEBUG_MSG(msg) \
    do { \
        if (DEBUG_MODE) { \
            std::cout << msg << std::endl; \
        } \
    } while(0)

void quicksort(std::vector<int>& arr, int left, int right);

#endif // QUICKSORT_H
