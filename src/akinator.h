#ifndef AKINATORlib
#define AKINATORlib

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

#include "list.h"
#include "color.h"
#include "graphic.h"
#include "tree.h"

TreeError Interaction();

TreeError   InteractionGuess(Tree* tree);
TreeError  InteractionDefine(Tree* tree, char* define);
TreeError InteractionCompare(Tree* tree, char* define1, char* define2);

TreeError  GuessObject(Tree* tree, Node** node);
TreeError  ComposePath(Tree* tree, Node*  node, Iterator* answer, char define[]);

TreeError DefineObject(Tree* tree);

TreeError  GiveDefine(Tree* tree, Iterator* answer, char* define);
TreeError GiveCompare(Tree* tree, Iterator* answer1, Iterator* answer2, char* define1, char* define2);
void GiveSign(Node* current, Iterator* answer, int value);


#endif