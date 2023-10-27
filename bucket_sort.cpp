#include "bucket_sort.h"

//Statement allows us to use the vector element with the 'std::' prefix within the functions that include this header.
using namespace std;

mutex lk; // Mutex for locking
// Define a vector to store thread pointers and the number of threads
//vector<thread*> threads;

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
        std::cout << "value: " << value << ", minVal: " << minVal << ", range: " << range << std::endl;
        // Calculate the index of the bucket for the current value
        int bucketIndex = (value - minVal) / range;

        // Ensure the bucketIndex is within bounds
        if (bucketIndex >= k) {
            bucketIndex = k - 1;
        }

        // Lock the mutex to ensure thread safety
        lk.lock();

        // Debug print statements
        std::cout << "bucketIndex: " << bucketIndex << std::endl;
        std::cout << "buckets size: " << buckets.size() << std::endl;
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
    // Determine the number of buckets (k)
    int n = input_array.size();

    // Set a reasonable default value for k (e.g., based on the number of elements)
    int k = max(1, n / 10);

    cout << "The number of buckets are " << k << endl;
    // Create k empty sorted lists (buckets) using maps
    vector<map<int, int>> buckets(k);

    // Calculate the size of each sub-array
    int subArraySize = n / numThreads;
    cout << "The subArraySize is " << subArraySize << endl;

    // Launch threads
    vector<thread> threads(numThreads);

    // Divide the input array into input array / numThreads
    for (int i = 0; i < numThreads; ++i) {
        // Calculate the start and end indices for the sub-array
        int startIndex = i * (n / numThreads);
        int endIndex = (i == numThreads - 1) ? n : (i + 1) * subArraySize;

        // Create a sub-array for this thread
        vector<int> subArray(input_array.begin() + startIndex, input_array.begin() + endIndex);

        // Calculate the minVal and maxVal for the sub-array
        int minVal, maxVal;

        if (!subArray.empty()) {
            cout << "subArray is not empty\n" << endl;
            minVal = *min_element(subArray.begin(), subArray.end());
            maxVal = *max_element(subArray.begin(), subArray.end());
        } else {
            cout << "subArray IS empty\n" << endl;
            // Handle the case when subArray is empty
            minVal = INT_MAX;
            maxVal = INT_MIN;
        }

        // Calculate the range for each bucket
        int range = (maxVal - minVal + 1) / k;
        cout << "minVal: " << minVal << ", range: " << range << endl;
        // Create threads and distribute subArray elements into buckets
        threads[i] = thread(Distribute, ref(subArray), minVal, ref(buckets), range, k);
    }

    // Join threads and clean up resources
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join(); // Join each thread
    }

    // Concatenate sorted buckets into the result vector
    vector<int> result;
    for (int i = 0; i < k; ++i) {
        for (const auto& pair : buckets[i]) {
            result.push_back(pair.second);
        }
    }
    return result;
}



