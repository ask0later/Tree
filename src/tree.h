#ifndef TREElib
#define TREElib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
//#include "readfile.h"
//#include "stack.h"


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
    NO_ERROR,       
    ERROR_RIGHT_BRACKET,
    ERROR_LEFT_BRACKET,
    ERROR_LOOP,
    ERROR_ALLOCATION,
    ERROR_CONST,
    FILE_NOT_OPEN,
    ERROR_POSITIONING_FUNC,
    DEFINE_IS_NULL,
    EXIT
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

struct Control
{
    Node* root;
    size_t size;
};

Error Constructor(Control* tree);

Error  StringConstructor(char* source, String* str);
Error   StringDestructor(String* str);
Error StringRedefinition(char* source, String* str);


Error InsertValue(Control* tree, Elem_t value);

Error  GuessObject(Control* tree, Node** node);
// Error DefineObject(Control* tree, Node* node, Stack answer, char define[]);



Error InputNodeData(Node** node);

Node*       New();
void     Delete(Node* node);
void Destructor(Control* tree);


void       TextDump(Control* tree);
Error   GraphicDump(Control* tree);

Error GraphicDumpNode(Node* node, size_t counter);
Error  PrintNode(Node* node, FILE*   To, Order order_value);

Error ReadTree(Node** node, FILE* From, Order order_value);

Error ReadPhrase(char* source, FILE* From);
void WriteToFile(Control* tree);

Error   CheckNoLoop(Control* tree);
Error NodeTraversal(Node* node, Node* addresses[], size_t counter);


Error Qsort(Node* addresses[], int first, int last);
Error  Swap(Node* addresses[], int left, int right);




void DumpErrors(Error error);

#endif


