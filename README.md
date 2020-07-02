# MapEmbed Hashing


## Introduction

Perfect hashing is a hash function that maps a set of distinct keys to a set of continuous distinct integer without collision. However, most existing perfect hash schemes are static, which means that they cannot support insertion operation, while most datasets in practice are dynamic.
To address this issue, we propose a novel hashing scheme, namely **MapEmbed Hashing**. 
Inspired by divide-and-conquer and map-and-reduce,
our key idea is named **map-and-embed**, including two phases: 1) Map all keys into many small virtual tables; 2) Embed all small tables into a large table by wrapping/rotating.
Our experimental results show that it can support fast incremental updates, and can achieve 90% load factor using an index of 0.5 bits per key.   


## About the source codes, dataset and parameter setting

We implement MapEmbed Hashing in C++. We complete these codes on Ubuntu SMP 16.04.1 and compiled successfully using GCC 5.4.0 and -O2 option. We also provide the source codes of the algorithms we compared with (Dynamic Perfect Hashing, and Minimal Perfect Hashing) in `./ComparedAlgorithms`.

We randomly generate 8M KV pairs. The key and the value fields of each KV pair are both 8B strings generated from a uniform distribution. 
If you want to use CAIDA datasets, please register in [CAIDA](http://www.caida.org/home/) first and then apply for the traces.

We set the slow memory size to the same as the size of the insertion workload (8M*16B), and set the fast memory size to 1.3MB (about 0.01 times of the slow memory). The other parameters setting is the same as mentioned in the paper.


# How to run

Suppose you've already cloned the repository and start from the 'MapEmbed' directory.

You just need:

	& make
	& ./MapEmbed


## Output format

Our program will print the load factor, bits per key, and the throuoghput of insertion and lookup of MapEmbed. 
After insertion stops, we perform an dynamic expansion operation, and then print the load factor and bits per key again. 
Note that to obtain convincing results of the throughput, you are supposed to set the micro "testcycles" in the main.cpp to a larger value (e.g. 100) and to run the program on a Linux server.