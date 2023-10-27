#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <climits>

// Declaring BucketSort function prototype
std::vector<int> BucketSort(std::vector<int>& input_array, int numThreads);

#endif // BUCKET_SORT_H
