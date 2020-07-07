#ifndef dph_h
#define dph_h

//This the implementation of Dynamic Perfect Hashing in C, referring to the paper of
//Martin Dietzfelbinger et. al, Dynamic Perfect Hashing: Upper and Lower Bounds(DPHULB).
//Hash is the main hash table, declared as a global variable. Efficiency of this code
//changes as parameters in functions as parameters c, SM, etc. change.

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "BOBHash32.h"


// ******************  debug ********************

int t = 0;
int hashcount = 0;
bool globalflag = 0;
int debugcount = 0;

// ******************  begin kv pair ********************
#define KEY_LEN 8
#define VAL_LEN 8
#define KV_NUM 400000

struct KV_entry{
    char key[KEY_LEN];
    char value[VAL_LEN];

    KV_entry(){
        memset(key, 0, KEY_LEN*sizeof(char));
        memset(value, 0, VAL_LEN*sizeof(char));
    }

    KV_entry& operator=(const KV_entry& e){
        memcpy(key, e.key, KEY_LEN*sizeof(char));
        memcpy(value, e.value, VAL_LEN*sizeof(char));
        return *this;
    }
    bool operator==(const KV_entry& e){
        return (memcmp(key, e.key, KEY_LEN*sizeof(char)) == 0);
    }
    bool operator==(const char* ekey){
        return (memcmp(key, ekey, KEY_LEN*sizeof(char)) == 0);
    }
    bool operator!=(const KV_entry& e){
        return (memcmp(key, e.key, KEY_LEN*sizeof(char)) != 0);
    }
};

//If position contains nothing or element is deleted, position will be set
//to DELETED, which is 0.
KV_entry DELETED = KV_entry();

KV_entry kvPairs[KV_NUM+10];

inline void random_string(char* str, int len){
    for(int i = 0; i < len; ++i)
        str[i] = '!' + rand()%93;
    str[len-1] = 0;
}

void create_random_kvs(KV_entry *kvs, int num){
    srand((uint32_t)time(0));
    for(int i = 0; i < num; ++i){
        random_string(kvs[i].key, KEY_LEN);
        random_string(kvs[i].value, VAL_LEN);
    }
}

// ******************  end kv pair ********************


typedef int Status;

//Data structure to store sub-hashtable
struct Bucket{
    KV_entry *element;
    uint32_t length;    // mj
    uint32_t number;    // bj
    uint32_t space;     // sj
    BOBHash32 t2e;

    Bucket(){
        uint32_t seed = rand() % MAX_PRIME32;
        t2e.initialize(seed);
    }
    int table_to_element(const char* key){
        return t2e.run(key, KEY_LEN*sizeof(char)) % space;
    }
    void reset_hash(){
        uint32_t seed = rand() % MAX_PRIME32;
        t2e.initialize(seed);
    }
};

//Data structure to store node in List
typedef struct Node{
    KV_entry element;
    struct Node *next;
}node;

//Data structure to store a list
struct list{
    node *head;
    uint32_t number;
};

typedef node * nodep;


#define SUCCESS 1
#define UNSUCCESS 0

// #define DELETED 

//If a Lookup operation finds the element it is looking for, it will return FOUND;
//otherwise NOT_FOUND.
#define FOUND 1
#define NOT_FOUND 0

//If a hash function is injective on the list, it will set the flag to NO_COLLISION;
//otherwise COLLISION.
#define NO_COLLISION 1
#define COLLISION 0


uint32_t cnt = 0;     //A variable records the number of operations(Insert, Delete).
uint32_t c = 2;         //A parameter used to resize M
uint32_t SM = 1;        //SM is a parameter to generate a s(M) function.
uint32_t M = 0;         //The main hash table is to accomodate up to M elements.
list * new_list;        //I have to declare the lists to store the elements temporarily
//as a global variable. As the test set grows larger, there can be
//issues of memories with the local one.


//Data structure to store the hash table
struct HashTable{
    Bucket *sub_table;
    uint32_t size;//s(M)
    BOBHash32 h2t;

    HashTable(){
        uint32_t seed = rand() % MAX_PRIME32;
        h2t.initialize(seed);
    }
    int hash_to_table(const char* key){
        return h2t.run(key, KEY_LEN*sizeof(char)) % size;
    } 
    void reset_hash(){
        uint32_t seed = rand() % MAX_PRIME32;
        h2t.initialize(seed);        
    }

    //Insert the key into the main hash table, Hash.
    Status Insert(KV_entry& kv){
        if(globalflag)
            debugcount++;

        cnt = cnt + 1;
        if (cnt > M){
            // printf("i: %d, load factor: %lf\n", t, (double)t/current_slots());
            RehashAll(kv);
        }
        else{
            //debug
            if(globalflag)
                hashcount += 1;

            uint32_t bucket_number = hash_to_table(kv.key);
            uint32_t location = sub_table[bucket_number].table_to_element(kv.key);
            if (kv == sub_table[bucket_number].element[location]){
                //If position contains key, then do nothing.
            }
            else{
                sub_table[bucket_number].number =  sub_table[bucket_number].number + 1;
                if (sub_table[bucket_number].number <= sub_table[bucket_number].length){
                    //size of sub-hashtable sufficient
                    if (sub_table[bucket_number].element[location] == DELETED){
                        //Store the key in the position, if it is deleted or the position is empty.
                        sub_table[bucket_number].element[location] = kv;
                    }
                    else{
                        //A ccollision occurs so that the sub-hashtable has to be rehashed
                        uint32_t i;
                        list temp_list;
                        temp_list.number = 0;
                        temp_list.head = (nodep)malloc(sizeof(node));
                        nodep node_pointer = temp_list.head;
                        
                        //Append all elements in the sub-hashtable to temp_list
                        for (i = 0; i < sub_table[bucket_number].space; i++){
                            if (sub_table[bucket_number].element[i] != DELETED) {
                                nodep temp = (nodep)malloc(sizeof(node));
                                temp->element = sub_table[bucket_number].element[i];
                                node_pointer->next = temp;
                                node_pointer = temp;
                                temp_list.number = temp_list.number + 1;
                                sub_table[bucket_number].element[i] = DELETED;
                            }
                        }
                        
                        //Append key to temp_list
                        nodep temp = (nodep)malloc(sizeof(node));
                        temp->element = kv;
                        temp->next = NULL;
                        node_pointer->next = temp;
                        temp_list.number = temp_list.number + 1;
                        
                        sub_table[bucket_number].number = temp_list.number;
                        int flag = 1;
                        int collision = NO_COLLISION;
                        uint32_t new_location;
                        
                        //Randomly choose hash function, until  it is injective on sub-hashtable, that is no collision
                        while (flag) {
                            node_pointer = temp_list.head->next;
                            sub_table[bucket_number].reset_hash();
                            
                            for (i = 0; i < temp_list.number; i++) {
                                new_location = sub_table[bucket_number].table_to_element(node_pointer->element.key);
                                if (sub_table[bucket_number].element[new_location] != DELETED &&
                                    sub_table[bucket_number].element[new_location] != node_pointer->element) {
                                    collision = COLLISION;
                                    break;
                                }
                                sub_table[bucket_number].element[new_location] = node_pointer->element;
                                node_pointer = node_pointer->next;
                            }
                            
                            
                            if (collision == NO_COLLISION)
                                flag = 0;
                            else{
                                collision = NO_COLLISION;
                                
                                for (i = 0; i < sub_table[bucket_number].space; i++)
                                    sub_table[bucket_number].element[i] = DELETED;
                                
                            }
                            
                        }
                    }
                }
                else{
                    //Sub-hashtable is too small
                    uint32_t i;
                    list temp_list;
                    temp_list.number = 0;
                    temp_list.head = (nodep)malloc(sizeof(node));
                    nodep node_pointer = temp_list.head;
                    
                    for (i = 0; i < sub_table[bucket_number].space; i++){
                        if (sub_table[bucket_number].element[i] != DELETED) {
                            nodep temp = (nodep)malloc(sizeof(node));
                            temp->element = sub_table[bucket_number].element[i];
                            node_pointer->next = temp;
                            node_pointer = temp;
                            temp_list.number = temp_list.number + 1;
                        }
                    }
                    
                    nodep temp = (nodep)malloc(sizeof(node));
                    temp->element = kv;
                    temp->next = NULL;
                    node_pointer->next = temp;
                    temp_list.number = temp_list.number + 1;
                    
                    //Double capacity of the sub-hashtable
                    if (sub_table[bucket_number].length > 1)
                        sub_table[bucket_number].length = 2 * sub_table[bucket_number].length;
                    else
                        sub_table[bucket_number].length = 2;
                    sub_table[bucket_number].space = 2 * sub_table[bucket_number].length * \
                    (sub_table[bucket_number].length - 1);
                    
                    //If condition ** is still satisfied, rehash the sub-hashtable
                    if (isSatisfied()) {
                        uint32_t new_size = sub_table[bucket_number].space;
                        sub_table[bucket_number].element = (KV_entry*)realloc(sub_table[bucket_number].element, \
                                                                        new_size * sizeof(KV_entry));
                        
                        for (i = 0; i < sub_table[bucket_number].space; i++){
                            sub_table[bucket_number].element[i] = DELETED;
                        }
                        
                        sub_table[bucket_number].number = temp_list.number;
                        
                        int flag = 1;
                        int collision = NO_COLLISION;
                        uint32_t new_location;
                        while (flag) {
                            node_pointer = temp_list.head->next;
                            sub_table[bucket_number].reset_hash();
                            
                            for (i = 0; i < temp_list.number; i++) {
                                new_location = sub_table[bucket_number].table_to_element(node_pointer->element.key);
                                if (sub_table[bucket_number].element[new_location] != DELETED &&
                                    sub_table[bucket_number].element[new_location] != node_pointer->element) {
                                    collision = COLLISION;
                                    break;
                                }
                                sub_table[bucket_number].element[new_location] = node_pointer->element;
                                node_pointer = node_pointer->next;
                            }
                            
                            if (collision == NO_COLLISION)
                                flag = 0;
                            else{
                                collision = NO_COLLISION;
                                for (i = 0; i < sub_table[bucket_number].space; i++)
                                    sub_table[bucket_number].element[i] = DELETED;
                            }
                        }
                        
                    }
                    else{
                        //Level-1 hash function is "bad"
                        RehashAll(kv);
                    }
                }
            }
        }
        return SUCCESS;
    }
    
    //s(M)function is chosen to be O(n), and this makes the total space used is linear in the
    //number of elements currently stored in the table. s(M) returns the new number of sub-hashtables.
    uint32_t s(uint32_t size){
        return (uint32_t)(SM * size);
    }

    //Rehashall is called by Insert, Delete or Initialize. It rehashes the whole hash table.
    Status RehashAll(KV_entry& kv){
        uint32_t i, k;
        uint32_t j = 0;
        list all_list;
        all_list.number = 0;
        all_list.head = (nodep)malloc(sizeof(node));
        nodep pointer = all_list.head;
        
        //Append all the elements in the hash table to all_list
        for(i = 0; i < size; i++){
            for (j = 0; j < sub_table[i].space; j++){
                if (sub_table[i].element[j] != DELETED) {
                    nodep temp = (nodep)malloc(sizeof(node));
                    temp->element = sub_table[i].element[j];
                    pointer->next = temp;
                    pointer = temp;
                    all_list.number = all_list.number + 1;
                }
            }
        }
        // debug
        if(globalflag)
            hashcount += size;

        free(sub_table);
        sub_table = NULL;
        
        if (kv != DELETED) {
            //If key is valid, append it to all_list
            nodep temp = (nodep)malloc(sizeof(node));
            temp->element = kv;
            temp->next = NULL;
            pointer->next = temp;
            all_list.number = all_list.number + 1;
        }

        cnt =  all_list.number;
        //Resize the whole hash table
        if (cnt > 4) {
            M = (1 + c) * cnt;
        }
        else{
            M = (1 + c) * 4;
        }
        
        new_list = (list *)malloc((s(M)) * sizeof(list));
        int flag = 1, m = 1;
        nodep node_pointer[s(M)];

        //Randomly choose level-1 hash function until the condition ** is satisfied.
        while (flag){
            for(i = 0; i < s(M); i++){
                new_list[i].number = 0;
                if (m == 1) {
                    new_list[i].head = (nodep)malloc(sizeof(node));
                }
            }
            
            reset_hash();
            size = s(M);
            
            for (i = 0; i < size; i++){
                node_pointer[i] = new_list[i].head;
            }
            
            nodep pointer = all_list.head->next;
            
            //Form every list for sub-table
            for (i = 0; i < all_list.number; i++){
                uint32_t list_num = hash_to_table(pointer->element.key);
                nodep temp = (nodep)malloc(sizeof(node));
                temp->element = pointer->element;
                new_list[list_num].number = new_list[list_num].number + 1;
                node_pointer[list_num]->next = temp;
                node_pointer[list_num] = temp;
                pointer = pointer->next;
            }
            
            sub_table = (Bucket *)realloc(sub_table, size * sizeof(Bucket));
            
            for (i = 0; i < size; i++){
                sub_table[i].number = new_list[i].number;
                if (new_list[i].number > 1){
                    sub_table[i].length = 2 * sub_table[i].number;
                    sub_table[i].space = 2 * sub_table[i].length * \
                    (sub_table[i].length - 1);
                }
                else{
                    sub_table[i].length = 2;
                    sub_table[i].space = 4;
                }
            }
            
            m++;
            if (isSatisfied()) {
                flag = 0;
            }
        }

        if(globalflag)
            hashcount += size;

        

        //Form every sub-hashtable
        for (i = 0; i < size; i++){
            int flag = 1;
            int collision = NO_COLLISION;
            uint32_t new_location;
            nodep pointer;
            sub_table[i].element = (KV_entry *)malloc(sub_table[i].space * sizeof(KV_entry));
            
            //Randomly choose hash function, until  it is injective on sub-hashtable, that is no collision
            while (flag) {
                sub_table[i].reset_hash();
                pointer = (new_list[i].head->next);
                
                for (j = 0; j < sub_table[i].space; j++)
                    sub_table[i].element[j] = DELETED;
                
                for (j = 0; j < new_list[i].number; j++) {
                    new_location = sub_table[i].table_to_element(pointer->element.key);
                    if (sub_table[i].element[new_location] != DELETED) {
                        collision = COLLISION;
                        break;
                    }
                    sub_table[i].element[new_location] = pointer -> element;
                    pointer = pointer -> next;
                }
                
                if (collision == NO_COLLISION )
                    flag = 0;
                else{
                    collision = NO_COLLISION;
                    for (k = 0; k < sub_table[i].space; k++)
                        sub_table[i].element[k] = DELETED;
                }
            }
        }
        return SUCCESS;
    }

    //Delete key from the hash table
    Status Delete(const char* key){
        cnt = cnt + 1;
        uint32_t bucket_number = hash_to_table(key);
        uint32_t location = sub_table[bucket_number].table_to_element(key);
        if (sub_table[bucket_number].element[location] == key)
            sub_table[bucket_number].element[location] = DELETED;
        else
            return UNSUCCESS;
        
        if (cnt > M){
            //Start a new phase
            RehashAll(DELETED);
        }
        
        return SUCCESS;
    }

    //Lookup key in hash table
    Status Lookup(const char* key){
        uint32_t bucket_number = hash_to_table(key);
        uint32_t location = sub_table[bucket_number].table_to_element(key);
        if (sub_table[bucket_number].element[location] == key){
            return FOUND;
        }
        return NOT_FOUND;
    }

    //Initialize the hash table
    Status Initialize(){
        cnt = 0;
        c = 2;         //A parameter used to resize M
        SM = 1;        //SM is a parameter to generate a s(M) function.
        M = 0;         //The main hash table is to accomodate up to M elements.
        reset_hash();
        size = 0;
        sub_table = NULL;
        RehashAll(DELETED);
        return SUCCESS;
    }

    //Check if condition ** is satisfied
    Status isSatisfied(){
        uint32_t i, sum = 0;
        uint32_t threshold = 32 * M * M / size + 4 * M;
        
        for (i = 0; i < size; i++) {
            sum += sub_table[i].space;
            if (sum > threshold) {
                //If sum already exceeds threshold, terminate the loop.
                return UNSUCCESS;
            }
        }
        
        return SUCCESS;
    }

    int current_slots(){
        int cnt = 0;
        for(int i = 0; i < size; ++i)
            cnt += sub_table[i].space;
        return cnt;
    }
}Hash;




#endif
