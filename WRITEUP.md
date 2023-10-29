# Introduction

## Core Challenges in Concurrent Programming

Concurrency programming revolves around addressing fundamental issues that arise when multiple threads or processes interact simultaneously. These issues include:

- **Avoiding Torn Accesses**: Ensuring the atomicity of write and read operations to prevent data corruption.
- **Coherence in Single-Location Accesses**: Making sense of accesses to a single memory location to maintain data integrity.
- **Memory Consistency in Multi-Location Accesses**: Managing memory accesses across multiple locations to ensure consistency.
- **Atomic Read-Modify-Write Operations**: Performing complex operations on shared data with atomicity.

### atomic_thread_fence(SEQ_CST); 
Whenever we have a store to load ordering, we put this call to prevent reordering during release consistency.

## Sorting Algrithm

### Intro to Bucket Sort

Bucket sort is a sorting technique that involves dividing elements into various groups, or buckets. These buckets are formed by uniformly distributing the elements. Once the elements are divided into buckets, they can be sorted using any other sorting algorithm. Finally, the sorted elements are gathered together in an ordered fashion.

This is a program mysort that written. 

## Lab write-up:
Your lab write-up, , called `WRITEUP.md`, will be longer this time.  In addition to the normal requirements, you should describe how changing the lock and barrier types in the two programs changes performance. In particular, your write-up should include:
* A table of all different locks (for both programs), which includes run time, L1 cache hit rate, branch-prediction hit rate, and page-fault count with four threads.
* The same table for all barriers.
* A discussion explaining why the best and worst primitives have these results.

Example; for markdown syntax see [link](https://markdown.land/markdown-table):
| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Some lock              | 2  | 50 | 50 | 50 |
| Another lock           | 2  | 50 | 50 | 50 |

Your write-up should also include the normal requirements:
* A description of your algorithms and challenges overcome
* A brief discussion of performance - what algorithm is faster? on what cases? why? 
* A brief description of your code organization
* A description of every file submitted
* Any extant bugs

I expect your lab write-up for this project will be longer, around 4 pages.

# Extant bugs

1. Peterson's lock works fine with DEBUG mode on and the margin for error is quite limited (.3%)
It doesn't work well when it isn't in Debug mode.

2. Peterson's locks do not work well at all without debug mode. The calulations are wayy off.



## References

1. https://www.geeksforgeeks.org/bucket-sort-2/
