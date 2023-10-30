/*****************************************************************
 * @author Suraj Ajjampur
 * @file bucket_sort.h
 * 
 * @brief This C++ header file used to include headers for bucket_sort.cpp
 * 
 * @date 28 Oct 2023
********************************************************************/

// Include guards
#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

// Headers
#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <climits>
#include <math.h>

/**
 * @brief Perform bucket sort on a vector of integers using multiple threads.
 *
 * This function sorts a given vector of integers using the bucket sort algorithm.
 * It distributes elements from the input vector into separate buckets based on
 * their values, and then sorts each bucket individually. Finally, it updates the
 * input vector with the sorted elements.
 *
 * @param A           The input vector of integers to be sorted.
 * @param NumThreads  The number of threads to be used for parallel processing.
 */
void BucketSort(std::vector<int>& A, int numThreads);


#endif // BUCKET_SORT_H
