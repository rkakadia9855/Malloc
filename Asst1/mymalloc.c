#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"
#define DEBUG 0

void print_memory_state() {
    struct metablock *iterator;
    iterator = memoryList;
    while(iterator != NULL) {
        printf("Block size: %ld\n", iterator->size);
        printf("Availability status: %d\n", (int) iterator->free);
        if(iterator->next == NULL)
            printf("Has next pointer: no\n");
        else
            printf("Has next pointer: yes\n");
        iterator = iterator->next;
    }
}

void initialize() {
 //   printf("initialize called\n");
    memoryList = (void *) myblock;
    memoryList->size = (4096 - sizeof(struct metablock));
    if(DEBUG)
        printf("size of memlist is %ld\n", memoryList->size);
    memoryList->free = 1;
    memoryList->next = NULL;
//    printf("initialized success\n");
}


// correctSlot: the pointer to the block in memory whose size is more than the size needed
// size: the size that is actually required
void split(struct metablock *correctSlot, size_t size) {
    // new metadata block created. its size will be 
    // equal to the sizeof the metablock the the size requested
    int tmp = (correctSlot->size)-size;
    struct metablock *new;
    if(tmp >= ( (int) sizeof(struct metablock))) {
        new = (void *)((void *)correctSlot+size+sizeof(struct metablock));
        new->size = (correctSlot->size)-size-sizeof(struct metablock);
        new->free = 1;
        new->next = correctSlot->next;
        if(DEBUG)
            printf("From split: Enough Available size after split: %d\n", tmp);
    }
    else {
        new = NULL;
        if(DEBUG)
            printf("From split: Not enough Available size after split: %d\n", tmp);
    }
    correctSlot->size = size;
    correctSlot->free = 0;
    correctSlot->next = new;
}

void *mymalloc(size_t numBytes, char *filename, int line) {
    if(DEBUG)
	    printf("trying to allocate %ld bytes\n", numBytes);
    if(numBytes == 0)
        return NULL;
    // the metablock pointers will be used to traverse through the list
    struct metablock *crnt, *prev;
    // the starting address of the allocated chunk of memory
    void *result;
    if(memoryList == NULL) {
        initialize();
    }
    crnt = memoryList;
    // we keep on traversing until we find a block that is free and of right size
    while((((crnt->size) < numBytes) || ((crnt->free) == 0)) && (crnt->next != NULL)) {
        if(DEBUG) {
            printf("available size: %ld\n", crnt->size);
            printf("required size: %ld\n", numBytes);
        }
        prev = crnt;
        crnt = crnt->next;
  //      printf("One block checked\n");
    }
    // found the perfect block. allocate the memory and return the pointer
    if((crnt->size) == numBytes  && crnt->free == 1) {
        crnt->free = 0;
        result = (void *)(++crnt);
        if(DEBUG)
            printf("Exact fitting block allocated\n");
        return result;
    }
    // if the chunk is of greater size than needed, split the chunk
    else if((crnt->size) > (numBytes+sizeof(struct metablock)) && crnt->free == 1) {
        split(crnt, numBytes);
        result = (void *)(++crnt);
        if(DEBUG)
            printf("Fitting block allocated with a split\n");
        return result;
    }
    else if(crnt == memoryList) {
        if((crnt->size) == numBytes  && crnt->free == 1) {
            crnt->free = 0;
            result = (void *)(++crnt);
            if(DEBUG)
                printf("Exact fitting block allocated\n");
            return result;
        }
        else if((crnt->size) > (numBytes) && crnt->free == 1) {
            split(crnt, numBytes);
            result = (void *)(++crnt);
            if(DEBUG)
                printf("Fitting block allocated with a split. block was initialized\n");
            return result;
        }
        else {
            result = NULL;
            printf("ERROR: No space is available to hold the block you requested.\n"
            "Filename: %s, Line: %d\n", filename, line);
            return result;
        }
    }
    else {
        result = NULL;
        printf("ERROR: No space is available to hold the block you requested.\n"
        "Filename: %s, Line: %d\n", filename, line);
        return result;
    }
}

// this function prevents external fragmentation. 
// this function joins the consecutive free blocks by removing the
//  metablocks lying in between them
void merge(){
 //   printf("entered merge\n");
    struct metablock *crnt, *prev;
    crnt=memoryList;
    //
    if(crnt != NULL && crnt->next == NULL) {
	crnt->size = 4096 - sizeof(struct metablock);
    }
    while((crnt != NULL) && ((crnt->next)!=NULL)){
        if((crnt->free) && (crnt->next->free)){
            crnt->size+=(crnt->next->size)+sizeof(struct metablock);
            crnt->next=crnt->next->next;
        }
        prev=crnt;
        crnt=crnt->next;
    }
}

void myfree(void* ptr, char *filename, int line){
	if(DEBUG)
		printf("Entered the free function\n");
    if(ptr == NULL) {
        printf("ERROR: Cannot free a null pointer.\n"
        "Filename: %s, Line: %d\n", filename, line);
        return;
    }
    if(memoryList == NULL) {
        printf("ERROR: Free call is invalid, since you haven't malloced anything\n"
        "Filename: %s, Line: %d\n", filename, line);
        return;
    }
 	if(DEBUG)
		printf("entering first if in free\n");
    if(((void*)myblock<=ptr)&&(ptr<=(void*)(myblock+4096))){
	    if(DEBUG)
	    	printf("entered first if in free\n");
        struct metablock* crnt=ptr;
        struct metablock *prev;
        struct metablock *tracker = memoryList;
	struct metablock *temp;
	temp = tracker + 1;
        int isBlock = 0;
        if((tracker + 1) == ptr) {
            isBlock = 1;
	        if(DEBUG)
		        printf("is block found before while\n");
	    }
        else {
            if(DEBUG) {
                printf("address of pointer to be freed: %p\n", &ptr);
                printf("address of tracker: %p\n", &(temp));
            }
        }
        while(!isBlock) {
	    if(DEBUG)
		    printf("entered while loop\n");
            prev = tracker;
            if(tracker != NULL) {
                tracker = tracker->next;
                if(tracker != NULL) {
                    if((tracker + 1) == ptr) 
                        isBlock = 1;
		    if(DEBUG)
			    printf("is block found inside while\n");
                }
                else
                    break;
            }
            else
                break;
        }
        if(isBlock) {
		if(DEBUG)
			printf("proceeding with operations after a block is confirmed\n");
            --crnt;
            if(crnt->free == 1) {
                printf("ERROR: The address you are trying to free is already free.\n"
                "Filename: %s, Line: %d\n", filename, line);
                return;
            }
            crnt->free=1;
        }
        else {
            printf("ERROR: The address you are trying to free was not malloced.\n"
                "Filename: %s, Line: %d\n", filename, line);
                return;
        }
	if(DEBUG)
		printf("entering merge from free\n");
        merge();
    }
    else {
         printf("ERROR: The address you are trying to free is out of bounds.\n"
        "Filename: %s, Line: %d\n", filename, line);
        return;
    }
    merge();
}

