# MapEmbed Hashing


## Introduction

Perfect hashing is a hash function that maps a set of distinct keys to a set of continuous distinct integers without collision. 
However, most existing perfect hash schemes are static, which means that they cannot support incremental updates, while most datasets in practice are dynamic.
To address this issue, we propose a novel hashing scheme, namely **MapEmbed Hashing**. 
Inspired by divide-and-conquer and map-and-reduce,
our key idea is named **map-and-embed** and includes two phases: 1) Map all keys into many small virtual tables; 2) Embed all small tables into a large table by circular move.
Our experimental results show that for the same experimental setting, the state-of-the-art hashing (dynamic perfect hashing) can achieve around 15% load factor, around 0.3 Mips update speed, while our MapEmbed achieves around 90%~95% load factor, and around 0.65 Mips update speed.


## About this repo

- `CPU` contains codes of MapEmbed and the related algorithms implemented on CPU platforms:
  - `MapEmbed`: Source codes for MapEmbed Hashing 
  - `ComparedAlgorithms`: Source codes for the related algorithms, including Dynamic Perfect Hashing and Static Perfect Hashing (CMPH)
- `FPGA` contains codes of MapEmbed implemented on FPGA platforms:
- more details can be found in the folders.




