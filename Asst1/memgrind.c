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
#define DEBUG 1

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

if(DEBUG)    
    printf("size of metadata block is %d\n", sizeof(struct metablock));


    //Workload A
    if(DEBUG)
	    printf("----------------------------------WORKLOAD A-----------------------------------\n");
    for( itr = 0; itr < 50; itr++) {
	 
        gettimeofday(&start_time, NULL);
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
        for(subItr = 0; subItr < 120; subItr++) {
            storeMallocPointers[subItr] = (int *) malloc(1);
        }
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
   /* for( itr = 0; itr < 50; itr++) {
        gettimeofday(&start_time, NULL);
        int prevOpFree = 0;
        int arrTracker = 0; //array index tracker
        int sizeTracker = 0; //array size tracker
        int totalMalloced = 0;
        int totalFreed = 0;
        for(subItr = 0; subItr < 240; subItr++) {
            if(sizeTracker == 0) {
                storeMallocPointers[arrTracker] = (int *) malloc(1);
                sizeTracker++;
                arrTracker++;
                totalMalloced++;
            }
            else if(sizeTracker == 120) {
                free(storeMallocPointers[sizeTracker - 1]);
                storeMallocPointers[sizeTracker - 1] = NULL;
                sizeTracker--;
                arrTracker--;
                totalFreed++;
            }
            else {
                time_t t;
                srand((unsigned) time(&t));
                int randomNum = rand() % 50;
                if(randomNum%3 != 0) {
                    if(totalMalloced < 120) {
                        storeMallocPointers[arrTracker] = (int *) malloc(1);
                        sizeTracker++;
                        arrTracker++;
                    }
                    else {
                        free(storeMallocPointers[sizeTracker - 1]);
                        storeMallocPointers[sizeTracker - 1] = NULL;
                        sizeTracker--;
                        arrTracker--;
                        totalFreed++;
                    }
                }
                else if(sizeTracker >= 1) {
                    free(storeMallocPointers[sizeTracker - 1]);
                    storeMallocPointers[sizeTracker - 1] = NULL;
                    sizeTracker--;
                    arrTracker--;
                    totalFreed++;
                }
            }
        }
        gettimeofday(&end_time, NULL);
        totalTimeC = totalTimeC + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
        (end_time.tv_usec-start_time.tv_usec));
    }
    */

    //Workload D
    if(DEBUG)
	    printf("---------------------------WORKLOAD D--------------------------------\n");
    for( itr = 0; itr < 50; itr++) {
        gettimeofday(&start_time, NULL);
        int *temp2;
        temp2 = (char *)malloc(20);
        free(temp2 + 10);
	free(temp2);
        int *tempSmt;
        free(tempSmt);
        int x;
        free((int *)x);
        gettimeofday(&end_time, NULL);
        totalTimeD = totalTimeD + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
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
        int *temp3;
        temp3 = (char *)malloc(20);
        free(temp3);
        free(temp3);
//	printf("memory state afte malloc and free of 20 bytes: \n");
//	print_memory_state();
        temp3 = (char *)malloc(20);
        free(temp3);
        temp3 = (char *)malloc(20);
        free(temp3);
//	printf("memory state after malloc and free of 20 bytes two times\n");
//	print_memory_state();
        temp3 = (char *) malloc(4072);
//	printf("memory state after malloc 4072 bytes\n");
//	print_memory_state();
        int *temp4;
        temp4 = (char *) malloc(1);
//	printf("memory state after malloc 1 byte\n");
//	print_memory_state();
        free(temp3);
	free(temp4);
        temp3 = (char *) malloc(4047);
        temp4 = (char *) malloc(1);
        free(temp3);
        free(temp4);
	/* printf("after malloc 4047 and malloc 1\n");
	print_memory_state(); */
        temp3 = (char *) malloc(4071);
        temp4 = (char *) malloc(1);
        free(temp3);
/*	printf("memory state after freeing 4071:\n");
	print_memory_state(); */
	free(temp4);
        gettimeofday(&end_time, NULL);
        totalTimeE = totalTimeE + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
            (end_time.tv_usec-start_time.tv_usec)); 
	printf("memroy state after a loop in E: \n");
	print_memory_state();
    } 
  if(DEBUG) {
        printf("Memory state after workload E: \n");
        print_memory_state();
    }
    printf("Mean time to execute workload A is %d milliseconds\n", totalTimeA/50);
    printf("Mean time to execute workload B is %d milliseconds\n", totalTimeB/50); 
  //  printf("Mean time to execute workload C is %d milliseconds\n", totalTimeC/50);
    printf("Mean time to execute workload D is %d milliseconds\n", totalTimeD/50);
    printf("Mean time to execute workload E is %d milliseconds\n", totalTimeE/50);
}
