#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <climits>
#include "my_atomics.h"
#include <math.h>


// Declaring BucketSort function prototype
void BucketSort(std::vector<int>& A, int numThreads);

#endif // BUCKET_SORT_H
