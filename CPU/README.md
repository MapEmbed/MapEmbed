# Codes on CPU Platform



## File Structure

- `MapEmbed`: The C++ source codes for MapEmbed Hashing
- `ComparedAlgorithms`: The source codes for the related algorithms, including Dynamic Perfect Hashing and Static Perfect Hashing (CMPH)



## Dependency

- **Murmur Hash**: The hash functions we use are 32-bit Murmur hash functions, obtained [here](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp).
- **g++**: We implement all the codes with C++ and build them with g++ (Ubuntu 7.5.0-6ubuntu2) 7.5.0 and -O3 option. 

## About the dataset 

The data we use in this repo are randomly generated KV pairs. We randomly generate 30M KV pairs. The key and the value fields of each KV pair are both 4-Bytes strings generated from a uniform distribution. 
If you want to use CAIDA datasets, please register in [CAIDA](http://www.caida.org/home/) first and then apply for the traces.



