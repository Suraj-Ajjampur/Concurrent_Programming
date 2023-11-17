/*****************************************************************
 * @author Suraj Ajjampur
 * @file quicksort.cpp
 * 
 * @brief OpenMP Multithreaded Quick Sort Algorrithm implementation in the recursive way 
 * 
 * @date 16 Nov 2023
 * 
 * @ref https://www.geeksforgeeks.org/multithreading-in-cpp/
********************************************************************/
#include "quicksort.h"

/**
 * @brief Partitions the given vector around a pivot.
 *
 * This function takes the last element as pivot, places the pivot element at its
 * correct position in the sorted array, and places all smaller elements (smaller than pivot)
 * to the left of pivot and all greater elements to the right of pivot.
 * 
 * @param arr Reference to the vector of integers to be sorted.
 * @param low_index The starting index of the subarray of arr.
 * @param high_index The ending index of the subarray of arr.
 * @return The index of the pivot element after partitioning.
 */
int partition(std::vector<int>& arr, int low_index, int high_index) {
    int pivot = arr[high_index]; // Choose the last element as the pivot
    int i = low_index - 1; // Initialize an index for elements less than or equal to the pivot

    for (int j = low_index; j < high_index; j++) {
        if (arr[j] <= pivot) {
            i++; // Increment the index for elements less than or equal to the pivot
            std::swap(arr[i], arr[j]); // Swap arr[i] and arr[j]
        }
    }
    std::swap(arr[i + 1], arr[high_index]); // Place the pivot in the correct position
    return i + 1;
}


/**
 * @brief Sorts a vector using the quicksort algorithm.
 *
 * This function sorts the elements of arr[] using the QuickSort algorithm.
 * It partitions the array into two halves, then recursively sorts the sub-arrays.
 * The function is parallelized with OpenMP, allowing the two halves of the array
 * to be sorted in parallel.
 *
 * @note The degree of parallelism is controlled externally by setting the number of threads
 * in OpenMP. The function will respect the number of threads set in the calling environment.
 *
 * @param arr Reference to the vector of integers to be sorted.
 * @param left The starting index of the subarray of arr to be sorted.
 * @param right The ending index of the subarray of arr to be sorted.
 */
void quicksort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(arr, left, right);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                quicksort(arr, left, pivotIndex - 1);
            }
            #pragma omp section
            {
                quicksort(arr, pivotIndex + 1, right);
            }
        }
    }
}
