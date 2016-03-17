# GridGraph
A large scale graph processing framework on a single machine.

## Compilation
Compilers supporting basic C++11 features (lambdas, threads, etc.) and OpenMP are required.

To compile:
```
make
```

## Preprocessing
Before running applications on a graph, GridGraph needs to partition the original edge list into the grid format.

Two types of edge list files are supported:
- Unweighted. Edges are tuples of <4 byte source, 4 byte destination>.
- Weighted. Edges are tuples of <4 byte source, 4 byte destination, 4 byte float typed weight>.

To partition the edge list:
```
./bin/preprocess -i [input path] -o [output path] -v [vertices] -p [partitions] -t [edge type: 0=unweighted, 1=weighted]
```
For example, we want to partition the unweighted [LiveJournal](http://snap.stanford.edu/data/soc-LiveJournal1.html) graph into a 4x4 grid:
```
./bin/preprocess -i /data/LiveJournal -o /data/LiveJournal_Grid -v 4847571 -p 4 -t 0
```

> You may need to raise the limit of maximum open file descriptors (./tools/raise\_ulimit\_n.sh).

## Running Applications
To run the applications, just give the path of the grid format and the memory budge (unit in GB), as well as other necessary program parameters (e.g. the starting vertex of BFS, the number of iterations of PageRank, etc.):

### BFS
```
./bin/bfs [path] [start vertex id] [memory budget]
```

### WCC
```
./bin/wcc [path] [memory budget]
```

### SpMV
```
./bin/spmv [path] [memory budget]
```

### PageRank
```
./bin/pagerank [path] [number of iterations] [memory budget]
```

For example, to run 20 iterations of PageRank on the (grid partitioned) [LiveJournal](http://snap.stanford.edu/data/soc-LiveJournal1.html) graph using a machine with 8 GB RAM:
```
./bin/pagerank /data/LiveJournal_Grid 20 8
```

## Resources
Xiaowei Zhu, Wentao Han and Wenguang Chen. [GridGraph: Large-Scale Graph Processing on a Single Machine Using 2-Level Hierarchical Partitioning](https://www.usenix.org/system/files/conference/atc15/atc15-paper-zhu.pdf). Proceedings of the 2015 USENIX Annual Technical Conference, pages 375-386.

To cite GridGraph, you can use the following BibTeX entry:
```
@inproceedings {zhu2015gridgraph,
author = {Xiaowei Zhu and Wentao Han and Wenguang Chen},
title = {GridGraph: Large-Scale Graph Processing on a Single Machine Using 2-Level Hierarchical Partitioning},
booktitle = {2015 USENIX Annual Technical Conference (USENIX ATC 15)},
year = {2015},
month = Jul,
isbn = {978-1-931971-225},
address = {Santa Clara, CA},
pages = {375--386},
url = {https://www.usenix.org/conference/atc15/technical-session/presentation/zhu},
publisher = {USENIX Association},
}
```
