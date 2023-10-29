#include "bucket_sort.h"
#include <algorithm>
//Statement allows us to use the vector element with the 'std::' prefix within the functions that include this header.
using namespace std;

mutex lk; // Mutex for locking
// Define a vector to store thread pointers and the number of threads
vector<thread*> threads;

/**
 * Distribute elements from the input array into buckets based on their values.
 * 
 * @param input_array Reference to the input array of integers.
 * @param minVal The minimum value in the input_array.
 * @param buckets Reference to the vector of map-based buckets for storing sorted elements.
 * @param range The range of values per bucket.
 * @param k The number of buckets.
 */
//thread(Distribute, ref(subArray), minVal, ref(buckets), range, k);
void Distribute(
    vector<int>& input_array,
    int minVal,
    vector<map<int, int>>& buckets,
    int range,
    int k
) {
    // Distribute elements into buckets
    for (int value : input_array) {
        DEBUG_MSG("Value: " << value);
        // Calculate the index of the bucket for the current value
        int bucketIndex = (value - minVal) / range;

        // Ensure the bucketIndex is within bounds
        if (bucketIndex >= k) {
            bucketIndex = k - 1;
        }

        // Lock the mutex to ensure thread safety
        lk.lock();

        // Debug print statements
        DEBUG_MSG("bucketIndex: " << bucketIndex);
        DEBUG_MSG("buckets size: " << buckets.size());
        // Place the value in the appropriate bucket (sorted map)
        buckets[bucketIndex][value] = value;
        lk.unlock(); // Unlock to release the lock
    }
}

/**
 * Perform BucketSort on a given input array with automatic determination of the number of buckets (k).
 * 
 * @param input_array Reference to the input array of integers to be sorted.
 * @param numThreads Number of threads to be used for parallel processing.
 * @return A vector of sorted integers.
 */
vector<int> BucketSort(vector<int>& input_array, int numThreads) {
    if (input_array.empty()) {
        cerr << "Error: Input array is empty" << endl;
        return vector<int>(); // Return an empty vector as an error indicator
    }
    // Determine the number of buckets (k)
    int num_of_buckets = input_array.size()/2;
    DEBUG_MSG("The number of buckets are " << num_of_buckets);

    // Find the maximum and minimum values in the input vector
    int minVal = input_array[0];
    int maxVal = input_array[0];
    for (int i = 1; i < input_array.size(); i++) {
        if (input_array[i] < minVal) {
            minVal = input_array[i];
        }
        if (input_array[i] > maxVal) {
            maxVal = input_array[i];
        }
    }
    cout << "MinVal " << minVal << endl;
    cout << "MaxVal " << maxVal << endl;

    int range = (maxVal - minVal + 1) / num_of_buckets;
    DEBUG_MSG("The range is " << range);

    // Create num_of_buckets empty sorted lists (buckets) using vector of vectors
    vector<vector<int>> buckets(num_of_buckets);

    DEBUG_MSG("Created num_of_buckets empty sorted lists (buckets) using vector of vectors");
    // Put vector elements in different buckets
    for (int i = 0; i < num_of_buckets; i++) {
        int bucketIndex = (input_array[i] - minVal) / range;
        DEBUG_MSG("BucketIndex " << bucketIndex << "Value of i is " << i);
        buckets[bucketIndex].push_back(input_array[i]);
    }
    DEBUG_MSG("Allocating to buffers is complete ");
    // Sort individual buckets
    for (int i = 0; i < num_of_buckets; i++) {
        sort(buckets[i].begin(), buckets[i].end());
    }
    DEBUG_MSG("Sorting inidividual buckets completed ");

    vector<int> result;
    DEBUG_MSG("Printing buckets[0][1] =" << buckets[0][1]);
    // Concatenate all buckets into result
    int index = 0;

    for (int i = 0; i < num_of_buckets; i++) {
        DEBUG_MSG("buckets[i].size() =" << buckets[i].size());
        for (int j = 0; j < buckets[i].size(); j++) {
            DEBUG_MSG("Bucket Element =" << buckets[i][j]);
            result.push_back(buckets[i][j]);
        }
    }
    DEBUG_MSG("Concatenating all buckets into result completed ");
    return result;
}

