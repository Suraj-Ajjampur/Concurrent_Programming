# Introduction

## Core Challenges in Concurrent Programming

Concurrency programming revolves around addressing fundamental issues that arise when multiple threads or processes interact simultaneously. These issues include:

- **Avoiding Torn Accesses**: Ensuring the atomicity of write and read operations to prevent data corruption.
- **Coherence in Single-Location Accesses**: Making sense of accesses to a single memory location to maintain data integrity.
- **Memory Consistency in Multi-Location Accesses**: Managing memory accesses across multiple locations to ensure consistency.
- **Atomic Read-Modify-Write Operations**: Performing complex operations on shared data with atomicity.

# Sorting Algrithm

## Intro to Bucket Sort

Bucket sort is a sorting technique that involves dividing elements into various groups, or buckets. These buckets are formed by uniformly distributing the elements. Once the elements are divided into buckets, they can be sorted using any other sorting algorithm. Finally, the sorted elements are gathered together in an ordered fashion.

 The BucketSort function divides the input vector into buckets and employs multi-threading to distribute elements into these buckets concurrently.

## Challenges

Since my older implementation of bucket sort had some extant bugs, I had to redo the implementation of bucket sort which took a lot of time.

## Intro to counter

The counter simply increments a global int cntr in critical sections. The are concurrency primitives such as locks and barriers implemented around it based on the user input.

Since this was an easier algorithm, I used it to test out the funtionality of all the custom concurrency primitives defines in my_atomics.cpp


# Performance

I measured the performance of both these programs using the perf tool.

Steps I took for this, 

Copied my entire directory to the linux machine using scp command

```
scp -r ./lab2-Suraj-Ajjampur suaj6464@ecen4313-fl23-0.int.colorado.edu:~
```

Logged into the machine using the SSH command

```
ssh suaj6464@ecen4313-fl23-1.int.colorado.edu
```

Gave the below arguements for the perf command and used it to obtain the below tables for performance for barriers and locks

```
perf stat -e branches -e branch-misses -e faults -e L1-dcache-load-misses -e L1-dcache-loads ./counter -o out.txt -t5 -n10000 --bar=pthread
```

## Performance Table 1

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| tas              | 0.0034   | 100 | 94.07 | 38 |
| ttas           | 0.0011  | 100 | 94.13 | 38 |
| ticket           | 0.0037  | 100 | 93.89 | 37 |
| mcs           | 0.0011  | 100 | 93.90 | 37 |
| pthread           | 0.0014  | 100 | 94.03 | 39 |
| peterson           | 0.0009  | 100 | 94.20 | 37 |
| petersonrel           | 0.0010  | 100 | 93.96 | 37 |

## Performance Table 2

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| pthread              | 0.0034   | 100 | 94.07 | 38 |
| sense              | 0.0030   | 100 | 93.29 | 38 |

## Performance Summary

The performance is quite similar for the concurrency primitives, however primitives with use a fetch and increment have a decreased performance.


# Code Organization

- `Makefile`: This makefile defines compilation rules and dependencies for two C++ executables: mysort and counter. It specifies the compiler (g++) and compiler flags (-pthread -O3 -std=c++2a -g). It lists the source files for each executable and their corresponding object files. The default target is to build both mysort and counter executables. Rules for building individual executables and compiling source files are defined, and there's also a clean rule to remove object files and executables.

- `main_sort.cpp`: The code's entry point is the main function, which reads command-line arguments, including the number of threads. It calls the 'bucketSortAndPrint`, which sorts and prints the input file using the selected algorithm. This function also keep track of the execution time.

- `bucket_sort.cpp`: This performs a multithreaded bucket sort, using each thread to insert a section of the input array into each buck using the Insert() function. This is where locks are used based on the user input.

- `main_counter.cpp`: This program is the entry point where command-line arguments, number of iterations, including the number of threads are read. It spawns the threads and calls the counter function which uses locks based on the user input

- `my_atomics.cpp`: This is where all the concurrency primitives are defined. They are declared as functions or classes using atomic variables. This is the main foundation of the program.

# Extant bugs

- There are currently no extant bugs to my knowledge, however the release consistency locks for mcs, ticket etc

- There can be a better calling of concurrency primitives instead of using the if else ladder such a command table with the funtion pointer of the locks stored with the string associated.

# Testing:

I've created a file called my_own_test_directory where I have copied all the test files from the lab1 assignment.
I also have written a testscript.sh which I updated incrementally to test the functionality of my sorting algorithm.

## References

1. https://www.geeksforgeeks.org/bucket-sort-2/
2. Udemy Bucket Sort
3. https://markdown.land/markdown-table
4. https://xiaoyanzhuo.github.io/2019/01/18/Perf-Tool.html
