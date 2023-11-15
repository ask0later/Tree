#ifndef GRAPHIClib
#define GRAPHIClib
#include "list.h"
#include "tree.h" 


void    GraphicDumpList(Iterator* it);
void CreateGraphicNodes(List* list);
void CreateGraphicLinks(List* list);

TreeError   GraphicDump(Tree* tree);
TreeError GraphicDumpNode(Node* node, size_t counter);



#endif