#ifndef LISTlib
#define LISTlib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include "color.h"

const int    POISON_COUNTER  =      -1;
const int    POISON          = INT_MAX;
const size_t NUM_NODES       =      10;
const size_t MIN_SIZE_LIST   =       5;
const size_t FICT_ELEM       =       1;
const size_t MAX_SIZE_LIST   =     101;
const size_t VALUE_REALLOC   =       2;


struct NodeList
{
    int   data;
    ssize_t  next;
    ssize_t  prev;
};

struct List
{
    NodeList*      nodes;
    size_t      size;
    size_t  num_elem;
    ssize_t     free;
    ssize_t   errors;
};

struct Iterator 
{
    List* list = {};
    ssize_t   index;
};


void       TextDumpList(Iterator* it);


void     ConstructorList(Iterator* it);
void      DestructorList(Iterator* it);
ssize_t   ListInsert(Iterator* it, int value);
ssize_t    ListErase(Iterator* it); 

int       DumpErrors(Iterator* it);

ssize_t       FindErrors(Iterator* it);
ssize_t      CheckMemory(Iterator* it);
ssize_t  СheckForLooping(List* list);
ssize_t       LogicCheck(List* list);


ssize_t  IncreaseRealloc(Iterator* it);
ssize_t    ReduceRealloc(Iterator* it);
ssize_t    Linearization(Iterator* it);


ssize_t Push_Back (Iterator* it, int value);
ssize_t Push_Front(Iterator* it, int value);
ssize_t  Pop_Back (Iterator* it);
ssize_t  Pop_Front(Iterator* it);

ssize_t      Begin(Iterator* it);
ssize_t        End(Iterator* it);
ssize_t       Next(Iterator* it);

int  GetValueList(Iterator* it);
ssize_t SetValueList(Iterator* it, int value);



ssize_t FindElemByValue(Iterator* it, int value);

void FillMemory(List* list, size_t start, size_t end);



#endif