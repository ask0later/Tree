#ifndef AKINATORlib
#define AKINATORlib

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>

#include "list.h"
#include "color.h"
#include "tree.h"

const int ENUM_ANSWER_SIZE =  5;
const size_t MAX_SIZE_NAME = 25;

enum Answer
{
    GUESS = 1,
    DEFINE,
    COMPARE,
    GRAPHIC,
    BASE
};



TreeError MainMenu(Tree* tree);
TreeError    Akinator(Tree* tree);
TreeError  Parameters(Tree* tree);

Answer GetAnswer(int x);

TreeError   InteractionGuess(Tree* tree);
TreeError  InteractionDefine(Tree* tree, char* define);
TreeError InteractionCompare(Tree* tree, char* define1, char* define2);
TreeError         SelectBase(Tree* tree);
TreeError           DrawTree(Tree* tree);

TreeError  GuessObject(Tree* tree, Node** node);
TreeError  ComposePath(Tree* tree, Node*  node, Iterator* answer, char define[]);

TreeError DefineObject(Tree* tree);

TreeError  GiveDefine(Tree* tree, Iterator* answer, char* define);
TreeError GiveCompare(Tree* tree, Iterator* answer1, Iterator* answer2, char* define1, char* define2);
void GiveSign(Node* current, Iterator* answer, int value);

void             CleanBuffer();


#endif