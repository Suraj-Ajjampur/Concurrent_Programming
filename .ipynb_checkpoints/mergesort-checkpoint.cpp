/*****************************************************************
 * @author Suraj Ajjampur
 * @file mergesort.cpp
 * 
 * @brief OpenMP Multithreaded Merge Sort Algorrithm implementation in the recursicve way 
 * 
 * @date 15 Nov 2023
 * 
 * @ref https://www.geeksforgeeks.org/multithreading-in-cpp/
********************************************************************/

// Includes
#include "mergesort.h"
#include <omp.h>

/** Merging 2 lists(havles) of a single array
 * 
 * @param low index of the array
 * 
 * @param mid index of the array
 * 
 * @param high index of the array
 */
void merge(vector<int>& arr, int left, int middle, int right) {
    int n1 = middle - left + 1; //Calculate the size of the left sub-array
    int n2 = right - middle; //Calculate the size of the right sub-array

    // Create temporary arrays to hold the left and right sub-arrays
    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    // Copy data from the main array to temporary arrays leftArr[] and rightArr[]
    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++) {
        rightArr[i] = arr[middle + 1 + i];
    }

    // Merge the temporary arrays back into main array i.e arr[left..right]
    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i]; // Copy the smaller element from the left sub-array
            i++;
        } else {
            arr[k] = rightArr[j]; // Copy the smaller element from the right sub-array
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftArr[], if any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightArr[], if any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

/** Recursive Merge Sort function 
 *  
 * @param Input Array (Passed by reference)
 * @param low index of the input array to start the sort
 * @param high index of the input array to start the sort
 */ 
void mergesort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // Find the middle point
        int middle = low + (high - low) / 2;

    #pragma omp parallel default(none) shared(arr, low, middle, high)
    {
        // Recursively sort the first and second halves
        #pragma omp sections
        {
            #pragma omp section
            mergesort(arr, low, middle);
            #pragma omp section
            mergesort(arr, middle + 1, high);
        }

        // Merge the sorted halves
        merge(arr, low, middle, high);
    }
    }
}

        
            