#include <iostream>
#include <cstring>
#include "DynamicPerfectHashing.h"
using namespace std;


// int current_slots(){
//     int cnt = 0;
//     for(int i = 0; i < Hash.size; ++i)
//         cnt += Hash.sub_table[i].space;
//     return cnt;
// }


int main(){
    create_random_kvs(kvPairs, KV_NUM);
    // Hash.Initialize();

    const int KV = 30000;
    int fails = 0;


    timespec time1, time2;
    long long resns = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &time1); 
    for(t = 0; t <= 29000; ++t){
        if(Hash.Insert(kvPairs[t]) == 0)
            fails++;
        if(fails == 8)
            break;
    }
    clock_gettime(CLOCK_MONOTONIC, &time2); 
    resns += (long long)(time2.tv_sec - time1.tv_sec) * 1000000000LL + (time2.tv_nsec - time1.tv_nsec); 
    double insert_mips = (double)1000.0 * t / resns; 
    printf("insertion speed %lf\n", insert_mips);

    // FILE* fw;
    // fw = fopen("dph.csv", "w+");
    // fprintf(fw, "# of KV pairs,Memory Usage (KB)\n");
    // for(t = 0; t <= 29000; ++t){
    //     if(Hash.Insert(kvPairs[t]) == 0)
    //         fails++;
    //     if(fails == 1000)
    //         break;
    //     if(t%2000 == 0){
    //         printf("t:%d, memory usage:%d (KB)\n", t, Hash.current_slots()*16/1000);
    //         fprintf(fw, "%d,%d\n", t, Hash.current_slots()*16/1000);
    //     }
    // }
    // fclose(fw);



    // int repeat = 0;

    // while(repeat < 1000){
    //     Hash.Initialize();
    //     for(int i = 0; i < KV; ++i){
    //         if(i >= 10000 && (double)i/current_slots() >= 0.1){
    //             globalflag = 1;
    //             printf("%d\n", i);
    //             Hash.Insert(kvPairs[i]);
    //             globalflag = 0;
    //             break;
    //         }
    //         else
    //             Hash.Insert(kvPairs[i]);
    //     }
    //     repeat++;
    // }
    // printf("%d\n", hashcount);
    // printf("%d\n", debugcount);


    //     if(Hash.Insert(kvPairs[i]) == 0)
    //         fails++;
    //     if(fails == 10)
    //         break;
    // }


    // printf("load factor: %lf\n", (double)KV/current_slots());

    // for(int i = 0; i < 10; ++i)
    //     printf("lookup %d : %d\n", i, Hash.Lookup(kvPairs[i].key));

    // int cnt = 0;
    
    return 0;
}
