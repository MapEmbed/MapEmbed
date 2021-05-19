#include <iostream>
#include <cmath>

#include "MapEmbed.h"


int basic_test(){
    printf("-------------------Begin basic_test-------------------\n");    
    int testcycles = 10;
/******************************* create MapEmbed ********************************/
    int layer = 3;
    int bucket_number = 500000;
    int cell_number[3];
    cell_number[0] = 2250000;
    cell_number[1] = 750000;
    cell_number[2] = 250000;
    int cell_bit = 4;
    
    MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);
    timespec time1, time2;
    long long resns = 0;

/****************************** create/read data *******************************/
    create_random_kvs_keyint(kvPairs, KV_NUM);
    int p = 0;
    int fails = 0;
    int qfails = 0;

/******************************** insert data **********************************/
    double insert_mops = 0;
    for(int i = 0; i < testcycles; ++i){
        resns = 0;
        MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);
        clock_gettime(CLOCK_MONOTONIC, &time1); 
        for(p = 0; p < KV_NUM; ++p){
            if(mapembed.insert(kvPairs[p]) == false)
                if(++fails >= 8)
                    break;
        }
        clock_gettime(CLOCK_MONOTONIC, &time2); 
        resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
        double test_insert_mops = (double)1000.0 * p / resns; 
        insert_mops += (test_insert_mops - insert_mops) / (i+1);
    }

    for(p = 0; p < KV_NUM; ++p){
        if(mapembed.insert(kvPairs[p]) == false)
            if(++fails >= 8)
                break;
    }

/******************************* look-up data *********************************/
    double query_mops = 0;
    for(int i = 0; i < testcycles; ++i){
        resns = 0;
        clock_gettime(CLOCK_MONOTONIC, &time1); 
        for(int i = 0; i <= p; ++i){
            if(mapembed.query(kvPairs[i].key) == false)
                qfails++;
        }
        clock_gettime(CLOCK_MONOTONIC, &time2); 
        resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
        double test_query_mops = (double)1000.0 * p / resns; 
        query_mops += (test_query_mops - query_mops) / (i+1);
    }

/******************************* print results *********************************/
    printf("inserted items: %d\n", mapembed.calculate_bucket_items());
    printf("load factor: %lf, bits per key: %lf\n", mapembed.load_factor(), mapembed.bit_per_item());
    printf("insertion Mops: %lf, lookup Mops: %lf\n", insert_mops, query_mops);
    printf("qfails: %d\n", qfails);

/***************************** dynamic expansion ******************************/
    mapembed.extend();
    fails = 0;
    resns = 0;
    int icnt = 0;
    clock_gettime(CLOCK_MONOTONIC, &time1); 
    for(; p < KV_NUM; ++p){
        if(mapembed.insert(kvPairs[p]) == false){
            if(++fails >= 8)
                break;
        }
        else icnt++;
    }
    clock_gettime(CLOCK_MONOTONIC, &time2); 
    resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
    insert_mops = (double)1000.0 * icnt / resns; 

/******************************* print results *********************************/
    printf("-------------------after expansion-------------------\n");
    printf("inserted items: %d\n", mapembed.calculate_bucket_items());
    printf("load factor: %lf, bits per key: %lf\n", mapembed.load_factor(), mapembed.bit_per_item());
    // printf("insertion Mops: %lf\n", insert_mops);
    printf("-------------------End basic_test--------------------\n");
    return 0;
}



int main(){
    create_random_kvs_keyint(kvPairs, KV_NUM);
    basic_test();
    return 0;
}