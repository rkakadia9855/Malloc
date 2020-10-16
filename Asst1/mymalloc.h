#include<stdio.h>
#include<stddef.h>

static char myblock[4096];

struct metablock{
 size_t size;
 int free;
 struct metablock *next; 
};

struct metablock *memoryList = (void*)myblock;

void initialize();
void split(struct metablock *correctSlot,size_t size);
void *malloc(size_t numBytes);
void merge();
void free(void* ptr);