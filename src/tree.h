#ifndef TREElib
#define TREElib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
//#include "readfile.h"


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
{
    NO_ERROR = 0 << 0,
    ERROR_1  = 1 << 0,
    ERROR_2  = 1 << 1,
    ERROR_3  = 1 << 2,
    ERROR_4  = 1 << 3,
    ERROR_5  = 1 << 4,
    ERROR_6  = 1 << 5
};

struct Node
{
    Elem_t   data;
    Node*    left;
    Node*   right;
};

struct Control
{
    Node* root;
    size_t size;
};

Error Constructor(Control* tree);

Error InsertValue(Control* tree, Elem_t value);
Error GuessObject(Control* tree, Node** node);



Error InputNodeData(Node** node, char* data);

Node*       New();
void     Delete(Node* node);
void Destructor(Control* tree);


void       TextDump(Control* tree);
Error   GraphicDump(Control* tree);

Error GraphicDumpNode(Node* node, size_t counter);
Error  PrintNode(Node* node, FILE*   To, Order order_value);

Error ReadTree(Node** node, FILE* From, Order order_value);
void WriteToFile(Control* tree);

Error   CheckNoLoop(Control* tree);
Error NodeTraversal(Node* node, Node* addresses[], size_t counter);


Error Qsort(Node* addresses[], int first, int last);
Error  Swap(Node* addresses[], int left, int right);




void DumpErrors(Error error);


#endif


