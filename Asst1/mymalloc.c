#include <stdio.h>
#include "mymalloc.h"

void initialize() {
    memoryList->size = (4096 - sizeof(struct memblock));
    memoryList->free = 1;
    memoryList->next = NULL;
}


// RK: is split even needed?
void split(struct memblock *correctSlot, size_t size) {
    struct memblock *new = (void *)((void *)correctSlot+size+sizeof(struct memblock));
    new->size = (correctSlot->size)-size-sizeof(struct memblock);
    new->free = 1;
    new->next = correctSlot->next;
    correctSlot->size = size;
    correctSlot->free = 0;
    correctSlot->next = new;
}

void *MyMalloc(size_t numBytes) {
    struct memblock *crnt, *prev;
    void *result;
    if(!(memoryList->size)) {
        initialize();
        printf("Memory initialized\n");
    }
    crnt = memoryList;
    while((((crnt->size) < numBytes) || ((crnt->free) == 0)) && (crnt->next != NULL)) {
        prev = crnt;
        crnt = crnt->next;
        printf("One block checked\n");
    }
    if((crnt->size) == numBytes) {
        crnt->free = 0;
        result = (void *)(++crnt);
        printf("Exact fitting block allocated\n");
        return result;
    }
    else if((crnt->size) > (numBytes+sizeof(struct memblock))) {
        // RK: is split even needed?
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

void merge(){
    struct memblock *crnt,*prev;
    crnt=memoryList;
    while((crnt->next)!=NULL){
        if((crnt->free) && (crnt->next->free)){
            crnt->size+=(crnt->next->size)+sizeof(struct memblock);
            crnt->next=crnt->next->next;
        }
        prev=crnt;
        crnt=crnt->next;
    }
}

void MyFree(void* ptr){
    if(((void*)myblock<=ptr)&&(ptr<=(void*)(myblock+4096))){
        struct memblock* crnt=ptr;
        --crnt;
        crnt->free=1;
        merge();
    }
    else printf("Please provide a valid pointer allocated by MyMalloc\n");
}

