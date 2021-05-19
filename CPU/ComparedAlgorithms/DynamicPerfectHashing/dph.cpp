#include <iostream>
#include <cstring>
#include "DynamicPerfectHashing.h"
using namespace std;


int main(int argc, char* argv[]){
    if(argc != 2)
        exit(-1);
    int kv_number = atoi(argv[1]);
    
    create_random_kvs(kvPairs, KV_NUM);
    int fails = 0;

    timespec time1, time2;
    long long resns = 0;
    
    int count_fail = 0;

    // HashTable Hash;

    /* ******************** insert ********************** */
    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(t = 0; t <= kv_number; ++t){
        if(Hash.Insert(kvPairs[t]) == 0)
            fails++;
        if(fails == 8)
            break;
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double insert_mops = (double)1000.0 * t / resns;
    
    /* ******************** query *********************** */
    fails = 0;
    clock_gettime(CLOCK_MONOTONIC, &time1);
    for(int j = 0; j <= t; ++j){
        if(Hash.Lookup(kvPairs[j].key) == 0)
            fails++;
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);
    resns = (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec);
    double query_mops = (double)1000.0 * t / resns;

    count_fail += fails;
        
    printf("-----------Total------------\n");
    printf("insertion speed %lf\n", insert_mops);
    printf("query speed %lf\n", query_mops);
    printf("fails: %d\n", count_fail);
    printf("load factor: %lf\n", (double)t/Hash.current_slots());
    printf("memory usage: %lf KB\n", (double)Hash.memory_usage()/1000);
    
    return 0;
}
