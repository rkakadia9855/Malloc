#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"
#define malloc(size_t numBytes) mymalloc(size_t numBytes, __FILE__, __LINE__)
#define free(void* ptr) myfree(void* ptr, __FILE__, __LINE__)

void initialize() {
    memoryList->size = (4096 - sizeof(struct metablock));
    memoryList->free = 1;
    memoryList->next = NULL;
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

void *malloc(size_t numBytes) {
    // the metablock pointers will be used to traverse through the list
    struct metablock *crnt, *prev;
    // the starting address of the allocated chunk of memory
    void *result;
    if(!(memoryList->size)) {
        initialize();
        printf("Memory initialized in %s on line %s\n", __FILE__, __LINE__);
    }
    crnt = memoryList;
    // we keep on traversing until we find a block that is free and of right size
    while((((crnt->size) < numBytes) || ((crnt->free) == 0)) && (crnt->next != NULL)) {
        prev = crnt;
        crnt = crnt->next;
        printf("One block checked\n");
    }
    // found the perfect block. allocate the memory and return the pointer
    if((crnt->size) == numBytes) {
        crnt->free = 0;
        result = (void *)(++crnt);
        printf("Exact fitting block allocated\n");
        return result;
    }
    // if the chunk is of greater size than needed, split the chunk
    else if((crnt->size) > (numBytes+sizeof(struct metablock))) {
        split(crnt, numBytes);
        result = (void *)(++crnt);
        printf("Fitting block allocated with a split\n");
        return result;
    }
    else {
        result = NULL;
        printf("Sorry. No sufficient memory to allocate\n");
        return result;
    }
}

// this function prevents external fragmentation. 
// this function joins the consecutive free blocks by removing the
//  metablocks lying in between them
void merge(){
    struct metablock *crnt,*prev;
    crnt=memoryList;
    while((crnt->next)!=NULL){
        if((crnt->free) && (crnt->next->free)){
            crnt->size+=(crnt->next->size)+sizeof(struct metablock);
            crnt->next=crnt->next->next;
        }
        prev=crnt;
        crnt=crnt->next;
    }
}

void free(void* ptr){
    if(((void*)myblock<=ptr)&&(ptr<=(void*)(myblock+4096))){
        struct metablock* crnt=ptr;
        --crnt;
        crnt->free=1;
        merge();
    }
    else printf("Please provide a valid pointer allocated by MyMalloc\n");
}

