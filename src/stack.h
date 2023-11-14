#ifndef STACKlib
#define STACKlib

#include <stdlib.h>
#include <climits>
#include <stdio.h>
#include <assert.h>

//#define VERIFY(stk, err) Verify(stk, err, __PRETTY_FUNCTION__, line, file)
//#define STACK_PUSH(value, stk) StackPush(value, stk, __LINE__, __FILE__)
//#define STACK_POP(stk) StackPop(stk, __LINE__, __FILE__)
//#define STACK_DUMP(stk) StackDump(stk, errors, func, line, file)
 
const int NUM_ERRORS = 14;
const int NUM_REG    =  3;

typedef int elem_t;
typedef unsigned long long int valera_t;


struct Stack
{
#ifdef VALERA_VERIFICATION
    valera_t leftValera;
#endif
    elem_t* data;
    int size;
    int capacity;

#ifdef HASH_VERIFICATION
    long unsigned int hash_buf;
    long unsigned int hash_struct;
#endif

#ifdef VALERA_VERIFICATION
    valera_t rightValera;
#endif 
}; 


enum ErrorStack
{
    NO_ERROR_STACK = 0,            
    NEGATIVE_CAPACITY = 1,      
    NEGATIVE_SIZE = 2,
    ERROR_NULL_STK = 4,
    ERROR_ARRAY_EXIT = 8,
    ERROR_EXTRA_MEM = 16,
    ERROR_MEMORY = 32,
    ERROR_ALLOC = 64,
#ifdef VALERA_VERIFICATION
    ERROR_LEFT_VALERA = 128,
    ERROR_RIGHT_VALERA = 256,
    ERROR_LEFT_BUF = 512,
    ERROR_RIGHT_BUF = 1024,
#endif
#ifdef HASH_VERIFICATION
    ERROR_HASH_STRUCT = 2048,  
    ERROR_HASH_BUFFER = 4096   
#endif
};

const int MIN_CAPACITY = 6;

int  StackCtor(Stack* stk);
void StackDtor(Stack* stk);

void StackPush(elem_t value, Stack* stk);
elem_t StackPop(Stack* stk);

void Re_Calloc(bool more_or_less, Stack* stk);

void VerifyCapacity(Stack* stk);
long unsigned int HashFunction(void* ptr, size_t size);
void StackRehash(Stack* stk);

void Verify(Stack* stk, int errors/*, const char* func, const int line, const char* file*/);
int ErrorRate(Stack* stk);

void DumpErrors(int error_num);
void StackDump(Stack* stk, int errors/*, const char* func, const int line, const char* file*/);
void PrintStack(Stack* stk);


void PrintStackData(Stack* stk, FILE* fp);

#endif