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

enum Error
{ //Tree
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
    FASLE,
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

Error ConstructorTree(Tree* tree);
void   DestructorTree(Tree* tree);

Node*   NewNode();
void DeleteNode(Node* node);



Error  StringConstructor(char* source, String* str);
Error   StringDestructor(String* str);


Error InsertValue(Tree* tree, Elem_t value);


Error InputNodeData(Node** node);




void       TextDump(Tree* tree);

Error  PrintNode(Node* node, FILE*   To, Order order_value);

Error ReadTree(Node** node, FILE* From, Order order_value);

Error ReadPhrase(char* source, FILE* From);
void WriteToFile(Tree* tree);

Error   CheckNoLoop(Tree* tree);
Error NodeTraversal(Node* node, Node* addresses[], size_t counter);


Error Qsort(Node* addresses[], int first, int last);
Error  Swap(Node* addresses[], int left, int right);




void DumpErrors(Error error);

#endif


