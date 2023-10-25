/*****************************************************************
 * @author Suraj Ajjampur
 * @file my_atomics.cpp
 * 
 * @brief This C++ source file implements atomic operations and memory orderings
 *        using C++ atomics for test-and-set, fetch-and-increment, compare-and-swap,
 *        and a variant of compare-and-swap with a return value.
 * @date 24 Oct 2023
********************************************************************/

#include "my_atomics.h"

bool tas(atomic<bool>& x, std::memory_order MEM){
 bool expected = false;
 bool desired = true;
 return x.compare_exchange_strong(expected, desired, MEM); 
}

void tas_lock(atomic<bool>& x){
    while(tas(x,SEQ_CST) == false){}
}

void tas_unlock(atomic<bool>& x){
    return x.store(false, SEQ_CST);
}

void ttas_lock(atomic<bool>& x){
    while(x.load() == true ||
    tas(x,SEQ_CST) == false){}
}

void ttas_unlock(atomic<bool>& x){
    return x.store(false, SEQ_CST);
}


void ticket_lock(atomic<int>& next_num, atomic<int>& now_serving){
    int my_num = fai(next_num, 1 , SEQ_CST);
    while(now_serving.load(SEQ_CST) != my_num){}
}

void ticket_unlock(atomic<int>& now_serving){
    fai(now_serving, 1 ,SEQ_CST);
}

int fai(atomic<int>& x, int amount, std::memory_order MEM){
 return x.fetch_add(amount, MEM); 
}

template <typename T>
bool cas(atomic<T>& x, T expected, T desired, std::memory_order MEM){
 T expected_ref = expected;
 return x.compare_exchange_strong(expected_ref, desired, MEM); 
}

template <typename T>
T vcas(atomic<T> &x, T expected, T desired, std::memory_order MEM){
 T expected_ref = expected;
 x.compare_exchange_strong(expected_ref, desired, MEM);
 return expected_ref;
}
