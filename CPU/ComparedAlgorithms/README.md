# Source Codes of the Compared Algorithms

## Dynamic Perfect Hashing

- `DynamicPerfectHashing.h`: The implementation of Dynamic Perfect Hashing in C++, referring to the paper of
Martin Dietzfelbinger et. al, Dynamic Perfect Hashing: Upper and Lower Bounds(DPHULB). 

- `dph.cpp`: An example to run Dynamic Perfect Hashing. We insert the randomly generated KV pairs into DPH until the 8<sup>th</sup> insertion failure. 

## Static Perfect Hashing

- `cmph-master`: The CMPH Library encapsulates the newest and more efficient algorithms in an easy-to-use, production-quality, fast API. Our experiments are conducted using the CMPH Library.

- `cmph.c`: An example to run CMPH. We compare MapEmbed Hashing with four Minimal Perfect Hashing schemes (BMZ, CMH, BDZ, and CHD) and two Perfect Hashing schemes (BDZ\_PH and CHD\_PH) in CMPH library. 

## Dependency

These codes are based on `./BOBHash32.h`. The codes for Static Perfect Hashing are depended on CAIDA datasets. If you want to use CAIDA datasets, please register in [CAIDA](http://www.caida.org/home/) first and then apply for the traces.