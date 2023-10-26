/*****************************************************************
 * @author Suraj Ajjampur
 * @file my_atomics.h
 * 
 * @brief This C++ header file defines atomic operation functions and memory orderings
 *        using C++ atomics for test-and-set, fetch-and-increment, compare-and-swap,
 *        and a variant of compare-and-swap with a return value.
 * 
 * @date 24 Oct 2023
 * 
 * @assignment Concurrent Programming Fall 2023 (Lab 2)
********************************************************************/

#ifndef MY_ATOMICS_H
#define MY_ATOMICS_H

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>

#define SEQ_CST std::memory_order_seq_cst
#define RELAXED std::memory_order_relaxed

using namespace std;

void tas_lock(atomic<bool>& x);
void tas_unlock(atomic<bool>& x);

void ttas_lock(atomic<bool>& x);
void ttas_unlock(atomic<bool>& x);

void ticket_lock(atomic<int>& next_num, atomic<int>& now_serving);
void ticket_unlock(atomic<int>& now_serving);

int fai(atomic<int>& x, int amount, std::memory_order MEM);

template <typename T>
bool cas(atomic<T>& x, T expected, T desired, std::memory_order MEM);

template <typename T>
T vcas(atomic<T>& x, T expected, T desired, std::memory_order MEM);

/**
 * @brief SenseBarrier class for synchronization among multiple threads.
 *
 * This class implements a barrier synchronization primitive known as the Sense Barrier.
 * It allows multiple threads to synchronize at a common point before proceeding further.
 */
class SenseBarrier {
public:
    /**
     * @brief Constructs a SenseBarrier with a given number of threads.
     *
     * @param numThreads The total number of threads that will synchronize using this barrier.
     */
    SenseBarrier(int numThreads);
    /**
     * @brief Arrives at the barrier and waits for all other threads to arrive.
     *
     * This function is called by each thread when it reaches the barrier. It ensures that
     * all threads reach the barrier before any of them proceed further.
     */
    void ArriveAndWait();

private:
    atomic<int> cnt;    // Counter to track arrivals, propagated to all threads
    atomic<int> sense;  // Sense barrier, propagated to all threads
    int N;              // Total number of threads
};

#endif // MY_ATOMICS_H
