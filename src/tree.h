#ifndef TREElib
#define TREElib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include "color.h"


const size_t MAX_SIZE_TREE = 32;
const size_t MAX_SIZE_ARG  = 64;
typedef char* Elem_t;

enum Order
{
    PRE_ORDER  = 1,
    IN_ORDER   = 2,
    POST_ORDER = 3,
};

enum TreeError
{
    NO_ERROR,       
    ERROR_RIGHT_BRACKET,
    ERROR_LEFT_BRACKET,
    ERROR_LOOP,
    ERROR_ALLOCATION,
    ERROR_CONST,
    FILE_NOT_OPEN,
    ERROR_POSITIONING_FUNC,
    DEFINE_IS_NULL,
    LIB_IS_EMPTY,
    ELEMENT_NOT_FOUND,
    EXIT
};

enum TRUEorFALSE
{
    FALSE,
    TRUE
};


struct String
{
    char*  data;
    size_t size;
};

struct Node
{
    String   str;
    Node*    left;
    Node*   right;
};

struct Tree
{
    Node* root;
    size_t size;
};

TreeError ConstructorTree(Tree* tree);
void       DestructorTree(Tree* tree);

Node*   NewNode();
void DeleteNode(Node* node);

TreeError  StringConstructor(char* source, String* str);
TreeError   StringDestructor(String* str);


TreeError InsertValue(Tree* tree, Elem_t value);
char*      ReadPhrase(FILE* File);


void       TextDump(Tree* tree);

TreeError  PrintNode(Node* node, FILE*   To, Order order_value);
TreeError  ReadTree(Node** node, FILE* From, Order order_value);



TreeError   CheckNoLoop(Tree tree);
TreeError NodeTraversal(Node* node, Node* addresses[], size_t counter);

TreeError Qsort(Node* addresses[], int first, int last);
TreeError  Swap(Node* addresses[], int left, int right);

void DumpErrors(TreeError error);

void WriteToFile(Tree* tree);

TreeError ReadTextPhrase(char* source, FILE* From);


#endif


