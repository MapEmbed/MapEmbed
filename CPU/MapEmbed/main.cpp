#include <iostream>
#include <cmath>

#include "MapEmbed.h"


void fast_slow_test(int totalBytes, int testcycles = 1){
    printf("-------------------Begin fast_slow_test-------------------\n");    
    double ratio; // fast to slow
    for(ratio = 0.001; ratio <= 0.0151; ratio += 0.001){
        int layer = 3;

        int fastBytes = ratio / (1 + ratio) * totalBytes;
        int cell_number[3];
        int cell_bit = 4;
        cell_number[2] = fastBytes * 8 / cell_bit / (1+3+9);
        cell_number[1] = 3*cell_number[2]; 
        cell_number[0] = 3*cell_number[1];

        int slowBytes = 1 / (1 + ratio) * totalBytes; 
        int bucket_number = slowBytes / N / (KEY_LEN + VAL_LEN); 

        MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);

        timespec time1, time2;
        long long resns = 0;

        /******************************** insert data **********************************/
        double insert_mips = 0;
        int fails = 0;
        int qfails = 0;  // make O2 happy
        int p = 0; 
        int FAIL_STOP = 10;

        for(int i = 0; i < testcycles; ++i){
            resns = 0;
            fails = 0;
            MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);
            clock_gettime(CLOCK_MONOTONIC, &time1); 
            for(p = 0; p < KV_NUM; ++p){
                if(mapembed.insert(kvPairs[p]) == false)
                    if(++fails >= FAIL_STOP)
                        break;
            }
            clock_gettime(CLOCK_MONOTONIC, &time2); 
            resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
            double test_insert_mips = (double)1000.0 * p / resns; 
            insert_mips += (test_insert_mips - insert_mips) / (i+1);
        }

        fails = 0;
        for(p = 0; p < KV_NUM; ++p){
            if(mapembed.insert(kvPairs[p]) == false)
                if(++fails >= FAIL_STOP)
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

        printf("load factor: %lf, bits per key: %lf\n", mapembed.load_factor(), mapembed.bit_per_item());
        printf("insertion mips: %lf, lookup mips: %lf\n", insert_mips, query_mips);
        printf("qfails: %d (print to avoid O2)\n", qfails);
    }
    printf("--------------------End fast_slow_test--------------------\n");    
}


void memory_usage_test(int workload){
    printf("--------------------Begin memory_usage_test--------------------\n");    
    int layer = 3;
    int bucket_number = 160;
    int cell_number[3];
    cell_number[0] = 1800000;
    cell_number[1] = 600000;
    cell_number[2] = 200000;
    int cell_bit = 4;

    MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);

    int qfails = 0;
    int fails = 0;

    for(int i = 0; i < workload; ++i){
        if(mapembed.insert(kvPairs[i]) == false){
            if(++fails == 8){
                mapembed.extend(false);
                fails = 0;
            }
        }
    }
    for(int i = 0; i < workload; ++i){
        if(mapembed.query(kvPairs[i].key) == false)
            qfails++;
    }

    double memory_usage = mapembed.bucket_number*sizeof(Bucket) + (double)(cell_number[0]+cell_number[1]+cell_number[2])*cell_bit/8.0;
    printf("workload: %d, memory usage: %lf KB, qfails: %d, load factor: %lf\n", workload, (double)memory_usage/1000, qfails, mapembed.load_factor());
    printf("--------------------End memory_usage_test--------------------\n");  
}



int basic_test(){
    printf("-------------------Begin basic_test-------------------\n");    
    int testcycles = 10;
/******************************* create MapEmbed ********************************/
    int layer = 3;
    int bucket_number = 500000 /32*32;
    int cell_number[3];
    cell_number[0] = 1800000;
    cell_number[1] = 600000;
    cell_number[2] = 200000;
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
    printf("inserted items: %d\n", mapembed.calculate_bucket_items());
    printf("load factor: %lf, bits per key: %lf\n", mapembed.load_factor(), mapembed.bit_per_item());
    printf("insertion mips: %lf, lookup mips: %lf\n", insert_mips, query_mips);
    printf("qfails: %d\n", qfails);

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
    printf("-------------------End basic_test--------------------\n");
    return 0;
}


void layer_test(int workload){
    printf("-----------Begin layer_test---workload:%d------------\n",workload);    
    int bucket_number = workload/N;
    int cell_number[3];
    int total_cells = 0.015*bucket_number*sizeof(Bucket)*2;
    
    int cell_bit = 4;
    int mm = 1; 
    int unit = 0;
    for(int layer = 1; layer <= 3; layer++){
        unit += mm;
        mm *= 3;
        cell_number[layer-1] = total_cells / unit;
        for(int l = layer-2; l >= 0; --l)
            cell_number[l] = 3*cell_number[l+1]; 

        MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);

        int qfails = 0;
        int fails = 0;

        for(int i = 0; i < workload; ++i){
            if(mapembed.insert(kvPairs[i]) == false){
                if(++fails == 8)
                    break;
            }
        }
        for(int i = 0; i < workload; ++i){
            if(mapembed.query(kvPairs[i].key) == false)
                qfails++;
        }
        double memory_usage = mapembed.bucket_number*sizeof(Bucket) + (double)(total_cells)*cell_bit/8.0;
        printf("Layer: %d, qfails: %d, memory_usage: %lf, load factor: %lf\n", layer, qfails, memory_usage, mapembed.load_factor());
    }
    printf("--------------------End layer_test--------------------\n");  
}

void bucket_number_test(int workload, int bn = 160){
    int layer = 3;
    int bucket_number = bn;
    int cell_number[3];
    cell_number[0] = 1800000;
    cell_number[1] = 600000;
    cell_number[2] = 200000;
    int cell_bit = 4;

    MapEmbed mapembed(layer, bucket_number, cell_number, cell_bit);

    int qfails = 0;
    int fails = 0;

    timespec time1, time2;
    long long resns = 0;

    int i = 0;
    clock_gettime(CLOCK_MONOTONIC, &time1); 
    for(i = 0; i < workload; ++i){
        if(mapembed.insert(kvPairs[i]) == false){
            if(++fails == 8)
                break;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &time2);     
    resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
    double insert_mips = (double)1000.0 * i / resns; 

    for(int i = 0; i < workload; ++i){
        if(mapembed.query(kvPairs[i].key) == false)
            qfails++;
    }
    printf("Layer: %d, qfails: %d, load factor: %lf, ", layer, qfails, mapembed.load_factor());
    printf("insertion speed: %lf\n", insert_mips);

}




int main(){
    create_random_kvs_keyint(kvPairs, KV_NUM);
    // basic_test();
    // fast_slow_test(50000*32*32); 
    // for(int i = 100000; i <= 1000000; i += 100000)
    //     memory_usage_test(i);
    int bn = 1024;
    while(bn <= 16384){
        bucket_number_test(1100000, bn);
        bn *= 2;
    }
    return 0;
}