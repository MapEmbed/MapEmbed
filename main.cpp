#include <iostream>
#include <cmath>

#include "MapEmbed.h"

int main(){

    int testcycles = 10;
/******************************* create MapEmbed ********************************/
    int layer = 3;
    int bucket_number = 8000000/16;
    int cell_number[3];
    cell_number[0] = 1800000;
    cell_number[1] = 600000;
    cell_number[2] = 200000;
    int cell_bit = 4;
    
    MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);
    timespec time1, time2;
    long long resns = 0;

/****************************** create/read data *******************************/
    create_random_kvs(kvPairs, KV_NUM);
    int p = 0;
    int fails = 0;
    int qfails = 0;

/******************************** insert data **********************************/
    double insert_mips = 0;
    for(int i = 0; i < testcycles; ++i){
        resns = 0;
        MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);
        clock_gettime(CLOCK_MONOTONIC, &time1); 
        for(p = 0; p < KV_NUM; ++p){
            if(mapembed.insert(kvPairs[p]) == false)
                if(++fails >= 10)
                    break;
        }
        clock_gettime(CLOCK_MONOTONIC, &time2); 
        resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
        double test_insert_mips = (double)1000.0 * p / resns; 
        insert_mips += (test_insert_mips - insert_mips) / (i+1);
    }

    for(p = 0; p < KV_NUM; ++p){
        if(mapembed.insert(kvPairs[p]) == false)
            if(++fails >= 10)
                break;
    }

/******************************* look-up data *********************************/
    double query_mips = 0;
    for(int i = 0; i < testcycles; ++i){
        resns = 0;
        clock_gettime(CLOCK_MONOTONIC, &time1); 
        for(int i = 0; i <= p; ++i){
            if(mapembed.query(kvPairs[i].key) == false)
                qfails++;
        }
        clock_gettime(CLOCK_MONOTONIC, &time2); 
        resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
        double test_query_mips = (double)1000.0 * p / resns; 
        query_mips += (test_query_mips - query_mips) / (i+1);
    }

/******************************* print results *********************************/
    printf("------------------------begin------------------------\n");    
    printf("inserted items: %d\n", mapembed.calculate_bucket_items());
    printf("load factor: %lf, bits per key: %lf\n", mapembed.load_factor(), mapembed.bit_per_item());
    printf("insertion mips: %lf, lookup mips: %lf\n", insert_mips, query_mips);


/***************************** dynamic expansion ******************************/
    mapembed.extend();
    fails = 0;
    resns = 0;
    int icnt = 0;
    clock_gettime(CLOCK_MONOTONIC, &time1); 
    for(; p < KV_NUM; ++p){
        if(mapembed.insert(kvPairs[p]) == false){
            if(++fails >= 10)
                break;
        }
        else icnt++;
    }
    clock_gettime(CLOCK_MONOTONIC, &time2); 
    resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
    insert_mips = (double)1000.0 * icnt / resns; 

/******************************* print results *********************************/
    printf("-------------------after expansion-------------------\n");
    printf("inserted items: %d\n", mapembed.calculate_bucket_items());
    printf("load factor: %lf, bits per key: %lf\n", mapembed.load_factor(), mapembed.bit_per_item());
    // printf("insertion mips: %lf\n", insert_mips);
    printf("-------------------------end-------------------------\n");
    return 0;
}
