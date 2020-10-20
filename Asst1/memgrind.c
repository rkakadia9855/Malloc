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

	
	printf("size of metadata block is %d\n", sizeof(struct metablock));


    //Workload A
 /*   for( itr = 0; itr < 50; itr++) {
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

    //Workload B
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
    } */

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

    //tempWorldload C
    gettimeofday(&start_time, NULL);
    int *workD;
   // workD = (char *) malloc(4047);
   // free(workD);
    workD = (char *) malloc(4048);
    free(workD);
    workD = (char *) malloc(4049);
//    workD = (char *) malloc(5000);
 //   workD = (char *) malloc(4096);
  //  int *workD2;
 //   workD2 = (char *) malloc(4000);
//    workD = (char *) malloc(50);
    gettimeofday(&end_time, NULL);
    totalTimeC = totalTimeC + ((end_time.tv_sec-start_time.tv_sec)*1000000 + (end_time.tv_usec-start_time.tv_usec));

    //Workload D
  /*  gettimeofday(&start_time, NULL);
    int *temp2;
    temp2 = (char *)malloc(20);
    free(temp2 + 10);
    int *tempSmt;
    free(tempSmt);
    gettimeofday(&end_time, NULL);
    totalTimeD = totalTimeD + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
        (end_time.tv_usec-start_time.tv_usec));*/

    //Workload E
/*    gettimeofday(&start_time, NULL);
    int *temp3;
    temp3 = (char *)malloc(20);
    free(temp3);
    free(temp3);
    temp3 = (char *)malloc(20);
    free(temp3);
    temp3 = (char *)malloc(20);
    free(temp3);
    gettimeofday(&end_time, NULL);
    totalTimeE = totalTimeE + ((end_time.tv_sec-start_time.tv_sec)*1000000 + 
        (end_time.tv_usec-start_time.tv_usec)); */
   /* printf("Mean time to execute workload A is %d milliseconds\n", totalTimeA/50);
    printf("Mean time to execute workload B is %d milliseconds\n", totalTimeB/50); */
    printf("Mean time to execute workload C is %d milliseconds\n", totalTimeC/50);
 //   printf("Mean time to execute workload D is %d milliseconds\n", totalTimeD/50);
//    printf("Mean time to execute workload E is %d milliseconds\n", totalTimeE/50);
}
