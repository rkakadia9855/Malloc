#include<stdio.h>
#include<stddef.h>

static char myblock[4096];

struct memblock{
 size_t size;
 int free;
 struct memblock *next; 
};

struct memblock *memoryList=(void*)myblock;

void initialize();
void split(struct memblock *correctSlot,size_t size);
void *MyMalloc(size_t numBytes);
void merge();
void MyFree(void* ptr);