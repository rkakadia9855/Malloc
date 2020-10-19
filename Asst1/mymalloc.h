#include<stdio.h>
#include<stddef.h>
#define malloc( x ) mymalloc( x, __FILE__, __LINE__)
#define free( x ) myfree( x, __FILE__, __LINE__)

static char myblock[4096];

struct metablock{
 size_t size;
 int free;
 struct metablock *next; 
};

struct metablock *memoryList;

void initialize();
void split(struct metablock *correctSlot,size_t size);
void *mymalloc(size_t numBytes, char *filename, int line);
void merge();
void myfree(void* ptr, char *filename, int line);
