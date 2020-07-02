#include <cmph.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max_ele 50000
#define max_len 8

const int v[7]={0,2,5,6,7,8};
const char* type[]={"CMPH_BMZ","CMPH_CHM","CMPH_BDZ","CMPH_BDZ_PH","CMPH_CHD_PH","CMPH_CHD"};
// Create minimal perfect hash function from in-memory vector
int main(int argc, char **argv)
{ 
    //generating test data
    char* vector[max_ele];
    char* value[max_ele];
    char tvec[8];
    char tval[8];
    int rcd[max_ele];
    int temp;
    unsigned int nkeys;
    FILE* tfp=fopen("data_input.txt","r");
    if(tfp==NULL)
    {
        printf("openfile error\n");
    }
    for(int i=0;i<max_ele;++i)
    {
        vector[i]=malloc(8*sizeof(char));
        for(int j=0;j<8;++j)
        {
            fscanf(tfp,"%d",&temp);
            if(temp==0)
            {
                temp=1;
            }
            vector[i][j]=(char)temp;
        }
        value[i]=malloc(8*sizeof(char));
        for(int j=0;j<8;++j)
        {
            fscanf(tfp,"%d",&temp);
            value[i][j]=(char)temp;
        }
    }
    fclose(tfp);
    FILE* r_fp=fopen("output.csv","w");
    fprintf(r_fp,"algo_type,nkeys,create_time,query_time,loading_rate\n");
    for(int k=0;k<6;++k)
    {
        for(nkeys=max_ele;nkeys<=max_ele;nkeys+=1000)
        {
            long create_time,query_time,hash_size;
            create_time=query_time=hash_size=0;
            int repeat_time=1000;
            for(int j=0;j<repeat_time;++j)
            {
                printf("%d-%d\n",k,j);
                FILE* mphf_fd = fopen("temp.mph", "w");
                // Source of keys
                cmph_io_adapter_t *source = cmph_io_vector_adapter((char **)vector, nkeys);

                struct timeval start,end;
                gettimeofday(&start, NULL);   

                //Create minimal perfect hash function using the brz algorithm.
                cmph_config_t *config = cmph_config_new(source);
                cmph_config_set_algo(config, v[k]);
                cmph_config_set_mphf_fd(config, mphf_fd);
                cmph_t *hash = cmph_new(config);
                cmph_config_destroy(config);
                cmph_dump(hash, mphf_fd); 
                cmph_destroy(hash); 
                fclose(mphf_fd);

                mphf_fd = fopen("temp.mph", "r");
                hash = cmph_load(mphf_fd);

                for(int i=0;i<nkeys;++i)
                {
                    const char *key = vector[i];
                    unsigned int id = cmph_search(hash, key, (cmph_uint32)strlen(key));
                    rcd[i]=id;
                    for(int j=0;j<8;++j)
                    {
                        tvec[j]=vector[i][j];
                    }
                    for(int j=0;j<8;++j)
                    {
                        tval[j]=value[i][j];
                    }
                }

                gettimeofday(&end, NULL);
                long t1= 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec-start.tv_usec;
                create_time+=t1;

                //Find key
                gettimeofday(&start, NULL);


                for(int i=0;i<nkeys;++i)
                {
                    const char *key = vector[i];
                    unsigned int id = cmph_search(hash, key, (cmph_uint32)strlen(key));
                    rcd[i]=id;
                    for(int j=0;j<8;++j)
                    {
                        tvec[j]=vector[i][j];
                    }
                    for(int j=0;j<8;++j)
                    {
                        tval[j]=value[i][j];
                    }
                }
                gettimeofday(&end, NULL);
                long t2= 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec-start.tv_usec;
                query_time+=t2;

                int cmax=0;
                for(int i=0;i<nkeys;++i)
                {
                    if(rcd[i]>cmax)
                    {
                        cmax=rcd[i];
                    }
                }
                hash_size+=cmax+1;

                //Destroy hash
                cmph_destroy(hash);
                cmph_io_vector_adapter_destroy(source);   
                fclose(mphf_fd);

            }
            fprintf(r_fp,"%s,%d,%lf,%lf,%lf\n",type[k],nkeys,(double)(nkeys*repeat_time)/(double)create_time,(double)(nkeys*repeat_time)/(double)query_time,(double)(nkeys*repeat_time)/(double)hash_size);
        }
    }
    for(int i=0;i<max_ele;++i)
    {
        free(vector[i]);
        free(value[i]);
    }
    fclose(r_fp);
}
