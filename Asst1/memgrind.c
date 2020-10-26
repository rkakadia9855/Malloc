#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include "mymalloc.h"
#define DEBUG 0

int main() {
    int itr = 0;
    int subItr = 0;
    struct timeval start_time;
    struct timeval end_time;
    int totalTimeA = 0;
    int totalTimeB = 0;
    int totalTimeC = 0;
    int totalTimeD = 0;
    int totalTimeE = 0;
    int *storeMallocPointers[120];

if(DEBUG) {
    printf("size of metadata block is %ld\n", sizeof(struct metablock));
}

//NOTE: all the temp variables are to store temporary pointers


//Workload A
if(DEBUG) {
    printf("----------------------------------WORKLOAD A-----------------------------------\n");
}
    for( itr = 0; itr < 50; itr++) {
	 
        gettimeofday(&start_time, NULL);
        // testing mallocing 1 byte 120 bytes and then immediately freeing it
        for(subItr = 0; subItr < 120; subItr++) {
            storeMallocPointers[subItr] = (int *) malloc(1);
            free(storeMallocPointers[subItr]);
            storeMallocPointers[subItr] = NULL;
        }
        gettimeofday(&end_time, NULL);
        totalTimeA = totalTimeA + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
        (end_time.tv_usec-start_time.tv_usec));
    }

    if(DEBUG) {
        printf("Memory state after workload A: \n");
        print_memory_state();
    }

    //Workload B
    if(DEBUG) 
	    printf("------------------------------------WORKLOAD B-----------------------------------\n");
    for( itr = 0; itr < 50; itr++) {
	 
        gettimeofday(&start_time, NULL);
        // mallocing 1 bytes for 120 times
        for(subItr = 0; subItr < 120; subItr++) {
            storeMallocPointers[subItr] = (int *) malloc(1);
        }
        // freeing the 1 bytes that were malloced
        for(subItr = 0; subItr < 120; subItr++) {
            free(storeMallocPointers[subItr]);
            storeMallocPointers[subItr] = NULL;
        }
        gettimeofday(&end_time, NULL);
        totalTimeB = totalTimeB + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
        (end_time.tv_usec-start_time.tv_usec));
    } 

    if(DEBUG) {
        printf("Memory state after workload B: \n");
        print_memory_state();
    }  

    //Workload C
    if(DEBUG) 
	    printf("------------------------------------WORKLOAD C-----------------------------------\n");
    for( itr = 0; itr < 50; itr++) {
        gettimeofday(&start_time, NULL);
        int numMalloced = 0;
        int mallocIndex = 0;
        int freeIndex = 0;
        srand(time(NULL));
        int getRandom = rand() % 2;
        // randomly chooses between mallocing 1 byte or freeing
        for(subItr = 0; subItr < 240; subItr++) {
            if(numMalloced == 0) {
                storeMallocPointers[mallocIndex] = (int *) malloc(1);
                getRandom = rand() % 2;
                numMalloced++;
                mallocIndex++;
                continue;
            }
            if(numMalloced == 120) {
                free(storeMallocPointers[freeIndex]);
                storeMallocPointers[freeIndex] = NULL;
                freeIndex++;
            }
            else {
                if(getRandom == 0) {
                    storeMallocPointers[mallocIndex] = (int *) malloc(1);
                    numMalloced++;
                    mallocIndex++;
                }
                else {
                    if(storeMallocPointers[freeIndex] != NULL && freeIndex < numMalloced) {
                        free(storeMallocPointers[freeIndex]);
                        storeMallocPointers[freeIndex] = NULL;
                        freeIndex++;
                    }
                    else {
                        if(numMalloced < 120) {
                            storeMallocPointers[mallocIndex] = (int *) malloc(1);
                            numMalloced++;
                            mallocIndex++;
                        }
                    }
                }
            }
            getRandom = rand() % 2;
        }
        gettimeofday(&end_time, NULL);
        totalTimeC = totalTimeC + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
            (end_time.tv_usec-start_time.tv_usec));
    }
    if(DEBUG) {
        printf("Memory state after workload C: \n");
        print_memory_state();
    }

    //Workload D
    if(DEBUG)
	    printf("---------------------------WORKLOAD D--------------------------------\n");
    for( itr = 0; itr < 50; itr++) {
        gettimeofday(&start_time, NULL);
        char *temp2;
        // trying to free a datablock but the pointer doesn't point to the start of the datablock
        temp2 = (char *)malloc(20);
        free(temp2 + 10);
	    free(temp2);
        // trying to free a null pointer
        int *tempSmt;
        free(tempSmt);
        // trying to free a pointer that is not actually a pointer
        int x;
        free((int *)x);

        // trying to free a free pointer
        char *temp3;
        temp3 = (char *)malloc(20);
        free(temp3);
        free(temp3);
        temp3 = (char *)malloc(20);
        free(temp3);
        temp3 = (char *)malloc(20);
        free(temp3);
        temp3 = (char *) malloc(4072);
        // try to malloc when there's no more space available
        char *temp4;
        temp4 = (char *) malloc(1);
        free(temp3);
	    free(temp4);
        temp3 = (char *) malloc(4047);
        temp4 = (char *) malloc(1);
        free(temp3);
        free(temp4);
        // trying to malloc when there's no more space available
        temp3 = (char *) malloc(4071);
        temp4 = (char *) malloc(1);
        free(temp3);
	    free(temp4);
        gettimeofday(&end_time, NULL);
        totalTimeD = totalTimeD  + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
            (end_time.tv_usec-start_time.tv_usec));
    }
   if(DEBUG) {
        printf("Memory state after workload D: \n");
        print_memory_state();
    }  

    //Workload E
    if(DEBUG)
	    printf("-------------------------------------------WORKLOAD E------------------------------\n");
    for( itr = 0; itr < 50; itr++) {
        gettimeofday(&start_time, NULL);
        int numBlocks = 1;
        int mallocIndex = 0;
        int freeIndex = 0;

        //Malloc until no more space
        int totalMalloced = 0;
        while((storeMallocPointers[mallocIndex] = malloc(170)) != NULL) {
            numBlocks++;
            mallocIndex++;
            totalMalloced++;
        }

        if(DEBUG) {
            printf("Num malloced in while loop: %d\n", mallocIndex);
        }

        // Free alternative blocks
        int totalFree = 0;
        for(freeIndex = 0; freeIndex < mallocIndex; freeIndex += 2) {
            free(storeMallocPointers[freeIndex]);
            storeMallocPointers[freeIndex] = NULL;
        }

        // Memory is fragmented now. largest free block would be of size 50
        // Total amount of free space might be more than 50, but we can only malloc 50
        char *smt;
        smt = malloc(195); 

        for(mallocIndex = 0; mallocIndex < numBlocks - 1; mallocIndex += 2) {
            storeMallocPointers[mallocIndex] = malloc(85);
        }

        smt = malloc(85);

        for(freeIndex = 0; freeIndex < numBlocks - 1; freeIndex += 1) {
            free(storeMallocPointers[freeIndex]);
            storeMallocPointers[freeIndex] = NULL;
        } 

        gettimeofday(&end_time, NULL);
        totalTimeE = totalTimeE + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
            (end_time.tv_usec-start_time.tv_usec)); 
    } 
  if(DEBUG) {
        printf("Memory state after workload E: \n");
        print_memory_state();
    } 
    printf("Mean time to execute workload A is %d milliseconds\n", totalTimeA/50);
    printf("Mean time to execute workload B is %d milliseconds\n", totalTimeB/50); 
    printf("Mean time to execute workload C is %d milliseconds\n", totalTimeC/50);
    printf("Mean time to execute workload D is %d milliseconds\n", totalTimeD/50);
    printf("Mean time to execute workload E is %d milliseconds\n", totalTimeE/50);
}
