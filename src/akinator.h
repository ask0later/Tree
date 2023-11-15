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

Error Interaction();

Error   InteractionGuess(Tree* tree);
Error  InteractionDefine(Tree* tree, char* define);
Error InteractionCompare(Tree* tree, char* define1, char* define2);

Error  GuessObject(Tree* tree, Node** node);
Error ComposePath(Tree* tree, Node* node, Iterator* answer, char define[]);

Error DefineObject(Tree* tree);

Error  GiveDefine(Tree* tree, Iterator* answer, char* define);
Error GiveCompare(Tree* tree, Iterator* answer1, Iterator* answer2, char* define1, char* define2);


#endif