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

// /**
//  * @brief A simple Test-and-Set lock implementation.
//  *
//  * This class provides a basic Test-and-Set lock, where a flag is used to indicate
//  * whether the lock is held (true) or unheld (false). When a thread tries to acquire
//  * the lock using `lock()`, it will spin in a loop until the lock becomes available.
//  * When the lock is released using `unlock()`, the flag is set to false, indicating
//  * that the lock is now unheld and available for other threads to acquire.
//  */
// class TestAndSetLock {
// public:
//     // Constructor to initialize the lock flag to false (unheld)
//     TestAndSetLock();

//     // Lock the resource by setting the flag to true (held)
//     void lock();

//     // Unlock the resource by setting the flag to false (unheld)
//     void unlock();

// private:
//     std::atomic<bool> flag; // Atomic boolean flag indicating lock status
// };

#endif // MY_ATOMICS_H
