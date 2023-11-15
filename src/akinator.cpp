#include "akinator.h"
#include "graphic.h"

const char*  lib   =  "lib.txt";

TreeError Interaction()
{
    int answer = 0;
    printf(GREEN_COLOR "Guess[1]" YELLOW_COLOR "Define[2]" BLUE_COLOR "Compare[3]\n" END_COLOR);
    if (scanf("%d", &answer) != 1) 
        Interaction();
    
    while (getchar() != '\n')   // clean buffer
        ;

    Tree tree = {};
    TreeError error;
    FILE*   File = fopen(lib, "r");   

    ReadTree(&tree.root, File, PRE_ORDER);
    fclose(File);
    
    error = CheckNoLoop(tree); 
    if (error != NO_ERROR)
    {
        DumpErrors(error);
        exit(1);
    }

    char define1[MAX_SIZE_ARG] = {};
    char define2[MAX_SIZE_ARG] = {};
    char* temp1;
    char* temp2;

    if (answer == 1)
    {
        error = InteractionGuess(&tree);
    }
    else if (answer == 2)
    {
        printf("Введите объект, определение которого вы хотите узнать\n");
        temp1 = ReadPhrase(stdin);
        memcpy(define1, temp1, strlen(temp1));
        free(temp1);
        error = InteractionDefine(&tree, define1);
    }
    else if (answer == 3)
    {
        printf("Введите объекты, сравнить которые вы хотите\n");
        temp1 = ReadPhrase(stdin);
        memcpy(define1, temp1, strlen(temp1));
        free(temp1);

        temp2 = ReadPhrase(stdin);
        memcpy(define2, temp2, strlen(temp2));
        free(temp2);

        error = InteractionCompare(&tree, define1, define2);
    }
    
    GraphicDump(&tree);

    DestructorTree(&tree);

    return error;
}

TreeError InteractionGuess(Tree* tree)
{
    int  value =  INT_MAX;
                                    
    TreeError error = GuessObject(tree, &((*tree).root));
    if ((error != NO_ERROR) && (error != EXIT))
    {
        DestructorTree(tree);
        DumpErrors(error);
        return error;
    }
    
    printf("Хотите вы записать в библиотеку Ваши добавленные объекты? [1] - да [0] - нет\n");
    if ((scanf("%d", &value) == 1) && (value == 1))
    {
        FILE* File = fopen(lib, "w");
        PrintNode(tree->root, File, PRE_ORDER);
        fclose(File);
    }
    
    GraphicDump(tree);

    return NO_ERROR;
}

TreeError InteractionDefine(Tree* tree, char* define)
{
    Iterator answer = {};
    
    ConstructorList(&answer);

    TreeError error = ComposePath(tree, tree->root, &answer, define);
    if (answer.list->num_elem == -1)
    {
        DestructorList(&answer);
        return ELEMENT_NOT_FOUND;
    }
    
    GiveDefine(tree, &answer, define);
    
    DestructorList(&answer);

    return NO_ERROR;
}

TreeError InteractionCompare(Tree* tree, char* define1, char* define2)
{
    Iterator answer1 = {};
    Iterator answer2 = {};

    ConstructorList(&answer1);
    ConstructorList(&answer2);


    TreeError error = ComposePath(tree, tree->root, &answer1, define1);
    if (answer1.list->num_elem == -1)
    {
        DestructorList(&answer1);
        DestructorList(&answer2);
        return ELEMENT_NOT_FOUND;
    }
    error = ComposePath(tree, tree->root, &answer2, define2);
    if (answer1.list->num_elem == -1)
    {
        DestructorList(&answer1);
        DestructorList(&answer2);
        return ELEMENT_NOT_FOUND;
    }

    GiveCompare(tree, &answer1, &answer2, define1, define2);

    DestructorList(&answer1);
    DestructorList(&answer2);

    return error;
}

TreeError GiveDefine(Tree* tree, Iterator* answer, char* define)
{
    int value;
    Node* current = tree->root;

    printf("%s -", define);
    
    while(answer->list->num_elem != 0)
    {
        value = (int) Pop_Front(answer);
        if (value == (int)true)
        {
            printf(" %s", current->str.data);
            current = current->left;
        }
        else if (value == (int)false)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        if (answer->list->num_elem != 0)
            printf(",");
    }

    printf(".\n");
    return NO_ERROR;
}

TreeError GiveCompare(Tree* tree, Iterator* answer1, Iterator* answer2, char* define1, char* define2)
{
    int value1;
    int value2;

    Node* current = tree->root;
    
    value1 = (int) Pop_Front(answer1);
    value2 = (int) Pop_Front(answer2);
    if (value1 == value2)
        printf("%s и %s -", define1, define2);
    while (value1 == value2)
    {
        if (value1 == (int)true)
        {
            printf(" %s", current->str.data);
            
            if (current->left != NULL)
                current = current->left;
        }
        else if (value1 == (int)false)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        value1 = (int) Pop_Front(answer1);
        value2 = (int) Pop_Front(answer2);
        printf(",");
    }

    Node* current1 = current;
    Node* current2 = current;
    printf(" но %s", define1);
    
    GiveSign(current1, answer1, value1);

    printf(",");

    printf(" а %s", define2);
    
    GiveSign(current2, answer2, value2);

    printf(".\n");

    return NO_ERROR;
}

void GiveSign(Node* current, Iterator* answer, int value)
{
    do
    {
        if (value == (int)true)
        {
            printf(" %s", current->str.data);
            current = current->left;
        }
        else if (value == (int)false)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        
        value = (int) Pop_Front(answer);
        if (answer->list->num_elem + 1 != 0)
            printf(",");
    } while (answer->list->num_elem + 1 != 0);

}

TreeError GuessObject(Tree* tree, Node** node)
{
    char  answer[MAX_SIZE_ARG] = {};

    printf("Это %s?\n", (*node)->str.data);
    scanf("%s", answer);
    TreeError error;

    if (strcmp(answer, "да") == 0)
    {
        if ((*node)->left == NULL)
        {
            printf("Похоже, я угадала!\n");
            return EXIT;
        }
        error = GuessObject(tree, &(*node)->left);
        if (error != NO_ERROR) {return error;}
    }
    else if (strcmp(answer, "нет") == 0)
    {
        if ((*node)->right == NULL)
        {
            printf("К сожалению я не знаю, что вы загадали, скажите что это...\n");

            while (getchar() != '\n')   // clean buffer
                ;

            (*node)->left = NewNode();

            if (((*node)->left) == NULL) {return ERROR_ALLOCATION;}
            
            free((*node)->left->str.data);
            (*node)->left->str.data = ReadPhrase(stdin);
            
            (*node)->right = NewNode();
            if (((*node)->right) == NULL) {return ERROR_ALLOCATION;}

            (*node)->right->str.data = strdup(((*node)->str).data);

            printf("Чем %s отличается от %s\n", (*node)->left->str.data, (*node)->str.data);            

            free((*node)->str.data);
            (*node)->str.data = ReadPhrase(stdin);

            printf("Хорошо, я это запомню\n");
        
            tree->size++;

            error = GuessObject(tree, &(tree->root));
            if (error != NO_ERROR) {return error;}
        }
        error = GuessObject(tree, &(*node)->right);
        if (error != NO_ERROR) {return error;}
    }
    else 
    {
        printf("Введите корректно\n");
        error = GuessObject(tree, node);
        if (error != NO_ERROR) {return error;}
    }
    return NO_ERROR;
}

TreeError ComposePath(Tree* tree, Node* node, Iterator* answer, char define[])
{
    if (define == NULL) {return DEFINE_IS_NULL;}
    TreeError error;
    if (node->left != NULL)
    {
        Push_Back(answer, (int)true);
        error = ComposePath(tree, node->left, answer, define);
        if (error != NO_ERROR) {return error;}
    }

    if (node->right != NULL)
    {
        Push_Back(answer, (int)false);
        error = ComposePath(tree, node->right, answer, define);
        if (error != NO_ERROR) {return error;}
    }

    if (strcmp(node->str.data, define) == 0 )
    {
        return EXIT;
    }
    else
        Pop_Back(answer);
    
    return NO_ERROR;
}

char* ReadPhrase(FILE* File)
{
    size_t arg = 0;
    char* source;

    getline(&source, &arg, File);
    char* ref = (char*) calloc(strlen(source), sizeof(char));
    memcpy(ref, source, strlen(source) - 1);
    free(source);

    return ref;
}