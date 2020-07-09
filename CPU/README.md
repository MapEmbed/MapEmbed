# Codes on CPU Platform



## File Structure

- `MapEmbed`: The C++ source codes for MapEmbed Hashing
- `ComparedAlgorithms`: The source codes for the related algorithms, including Dynamic Perfect Hashing and Static Perfect Hashing (CMPH)



## Dependency

- **BOB Hash**: The hash functions we use are 32-bit Bob hash functions, obtained [here](http://burtleburtle.net/bob/hash/evahash.html).
- **g++**: We implement all the codes in C++. We compile these codes on Ubuntu SMP 16.04.1 and compiled successfully using GCC 5.4.0 and -O2 option. 

## About the dataset 

The data we use here are randomly generated KV pairs. We randomly generate 8M KV pairs. The key and the value fields of each KV pair are both 8-Bytes strings generated from a uniform distribution. 
If you want to use CAIDA datasets, please register in [CAIDA](http://www.caida.org/home/) first and then apply for the traces.



