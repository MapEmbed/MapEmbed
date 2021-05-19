# MapEmbed Hashing



## About the implementation, dataset, and parameter setting

We implement all the codes of MapEmbed Hashing with C++ and build them with g++ (Ubuntu 7.5.0-6ubuntu2) 7.5.0 and -O3 option. We also provide the source codes of the algorithms we compared with (Dynamic Perfect Hashing, and Static Perfect Hashing) in `../ComparedAlgorithms`.

We randomly generate 30M KV pairs. The key and the value fields of each KV pair are both 4B strings generated from a uniform distribution. 
If you want to use CAIDA datasets, please register in [CAIDA](http://www.caida.org/home/) first and then apply for the traces.

We set the slow memory size to about 27% of the size of the insertion workload (144MB), and set the fast memory size to 1.625MB (about 0.01 times of the slow memory). The other parameters setting is the same as mentioned in the paper. 

We sequentially insert the KV pairs in our hash table until the 8<sup>th</sup> insertion failure happens. After insertion stops, we print the relative statistics. Then we perform an dynamic expansion operation. Next, we repeat the preceding insertion operations and print the statistics again at the 8<sup>th</sup> insertion failure. 


# How to run

Suppose you've already cloned the repository and start from the `./CPU/MapEmbed` directory.

You just need:

	$ make
	$ ./MapEmbed


## Output format

Our program will print the load factor, bits per key, and the throuoghput of insertion and lookup of MapEmbed. 
Note that to obtain convincing results of the throughput, you are supposed to set the micro "testcycles" in the main.cpp to a larger value (e.g. 100) and to run the program on a Linux server.