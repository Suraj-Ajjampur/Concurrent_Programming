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