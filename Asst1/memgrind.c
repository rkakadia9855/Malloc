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
    int *storeMallocPointers[120];
   // printf("beginning 50 iterations\n");
    for( itr = 0; itr < 50; itr++) {

    printf("itr number %d\n", itr);
        //Workload A
/*	printf("starting worload A\n");
        for(subItr = 0; subItr < 120; subItr++) {
            printf("sub itr number %d\n", subItr);
	//    printf("allocating 1 byte from A\n");
            storeMallocPointers[subItr] = (int *) malloc(1);
	 //   printf("freeing from A\n");
            free(storeMallocPointers[subItr]);
	  //  printf("Freed from A\n");
            storeMallocPointers[subItr] = NULL;
        }
	printf("workload A completed\n");*/

/*	printf("starting workload B\n");
        //Workload B
        for(subItr = 0; subItr < 120; subItr++) {
            printf("sub itr number %d\n", subItr);
            storeMallocPointers[subItr] = (int *) malloc(1);
        }
        for(subItr = 0; subItr < 120; subItr++) {
            printf("sub itr number %d\n", subItr);
            free(storeMallocPointers[subItr]);
            storeMallocPointers[subItr] = NULL;
        }
	printf("workload B completed\n"); */ 

	printf("starting workload C\n");
        //Workload C
        int prevOpFree = 0;
        int arrTracker = 0; //array index tracker
        int sizeTracker = 0; //array size tracker
        int totalMalloced = 0;
        int totalFreed = 0;
        for(subItr = 0; subItr < 240; subItr++) {
            printf("sub itr number %d\n", subItr);
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
        printf("number of malloced: %d\n", totalMalloced);
        printf("number of freed: %d\n", totalFreed);
	printf("workload C completed\n"); 
    }
}
