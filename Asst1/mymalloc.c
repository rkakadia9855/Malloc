#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"

int initialized = 0;

void initialize() {
 //   printf("initialize called\n");
    memoryList = (void *) myblock;
    memoryList->size = (4096 - sizeof(struct metablock));
    memoryList->free = 1;
    memoryList->next = NULL;
//    printf("initialized success\n");
}


// correctSlot: the pointer to the block in memory whose size is more than the size needed
// size: the size that is actually required
void split(struct metablock *correctSlot, size_t size) {
    // new metadata block created. its size will be 
    // equal to the sizeof the metablock the the size requested
    struct metablock *new = (void *)((void *)correctSlot+size+sizeof(struct metablock));
    new->size = (correctSlot->size)-size-sizeof(struct metablock);
    new->free = 1;
    new->next = correctSlot->next;
    correctSlot->size = size;
    correctSlot->free = 0;
    correctSlot->next = new;
}

void *mymalloc(size_t numBytes, char *filename, int line) {
//	printf("entered mymalloc\n");
    if(numBytes == 0)
        return NULL;
    // the metablock pointers will be used to traverse through the list
    struct metablock *crnt, *prev;
    // the starting address of the allocated chunk of memory
    void *result;
 //   printf("entering first if statement to check if need to initialize\n");
    if(memoryList == NULL) {
        initialize();
    //    printf("Memory initialized in %s on line %d\n", __FILE__, __LINE__);
	initialized = 0;
    }
    crnt = memoryList;
    // we keep on traversing until we find a block that is free and of right size
    while((((crnt->size) < numBytes) || ((crnt->free) == 0)) && (crnt->next != NULL)) {
        prev = crnt;
        crnt = crnt->next;
  //      printf("One block checked\n");
    }
    // found the perfect block. allocate the memory and return the pointer
    if((crnt->size) == numBytes) {
        crnt->free = 0;
        result = (void *)(++crnt);
  //      printf("Exact fitting block allocated\n");
        return result;
    }
    // if the chunk is of greater size than needed, split the chunk
    else if((crnt->size) > (numBytes+sizeof(struct metablock))) {
        split(crnt, numBytes);
        result = (void *)(++crnt);
  //      printf("Fitting block allocated with a split\n");
        return result;
    }
    else {
        result = NULL;
        printf("ERROR: No space is available to hold the block you requested.\n
        Filename: %s, Line: %d\n", filename, line);
        return result;
    }
}

// this function prevents external fragmentation. 
// this function joins the consecutive free blocks by removing the
//  metablocks lying in between them
void merge(){
 //   printf("entered merge\n");
    struct metablock *crnt,*prev;
    crnt=memoryList;
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
    if(ptr == NULL) {
        printf("ERROR: Cannot free a null pointer.\n
        Filename: %s, Line: %d\n", filename, line);
        return;
    }
    if(memoryList == NULL) {
        printf("ERROR: Free call is invalid, since you haven't malloced anything\n
        Filename: %s, Line: %d\n", filename, line);
        return;
    }
 //   printf("entered free\n");
    if(((void*)myblock<=ptr)&&(ptr<=(void*)(myblock+4096))){
//	    printf("entered if in free\n");
        struct metablock* crnt=ptr;
        --crnt;
        if(crnt->free == 1) {
            printf("ERROR: The address you are trying to free is already free.\n
            Filename: %s, Line: %d\n", filename, line);
            return;
        }
        crnt->free=1;
//	printf("entering merge from free\n");
        merge();
    }
    else {
         printf("ERROR: The address you are trying to free is out of bounds.\n
        Filename: %s, Line: %d\n", filename, line);
        return;
    }
}

