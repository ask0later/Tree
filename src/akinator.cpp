#include "akinator.h"

const char*  lib   =  "lib.txt";

Error Interaction()
{
    int answer = 0;
    printf(GREEN_COLOR "Guess[1]" YELLOW_COLOR "Define[2]" BLUE_COLOR "Compare[3]\n" END_COLOR);
    if (scanf("%d", &answer) != 1) 
        Interaction();
    
    printf("%d\n", answer);
    while (getchar() != '\n')   // clean buffer
        ;

    Tree tree = {};
    Error error;
    FILE*   File = fopen(lib, "r");   

    ReadTree(&tree.root, File, PRE_ORDER);
    fclose(File);
    
    // PrintNode(tree.root, stderr, PRE_ORDER);


    // error = CheckNoLoop(&tree); //ВАТАФАК
    // if (error != NO_ERROR)
    // {
    //     DumpErrors(error);
    //     exit(1);
    // }

    char define1[MAX_SIZE_ARG] = {};
    char define2[MAX_SIZE_ARG] = {};
    char* temp1;
    char* temp2;
    size_t n = 0;
    if (answer == 1)
    {
        error = InteractionGuess(&tree);
    }
    else if (answer == 2)
    {
        getline(&temp1, &n, stdin);
        memcpy(define1, temp1, strlen(temp1) - 1);
        free(temp1);
        error = InteractionDefine(&tree, define1);
    }
    else if (answer == 3)
    {
        getline(&temp1, &n, stdin);
        memcpy(define1, temp1, strlen(temp1) - 1);
        free(temp1);

        getline(&temp2, &n, stdin);
        memcpy(define2, temp2, strlen(temp2) - 1);
        free(temp2);

        error = InteractionCompare(&tree, define1, define2);
    }
    
    DestructorTree(&tree);

    return error;
}

Error InteractionGuess(Tree* tree)
{
    int  value =  INT_MAX;
                                    
    Error error = GuessObject(tree, &((*tree).root));
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
    
    //GraphicDump(tree);

    return NO_ERROR;
}

Error InteractionDefine(Tree* tree, char* define)
{
    Iterator answer = {};
    
    ConstructorList(&answer);

    Error error = ComposePath(tree, tree->root, &answer, define);
    printf("%d\n", answer.list->num_elem);
    if (answer.list->num_elem == -1)
    {
        DestructorList(&answer);
        return ELEMENT_NOT_FOUND;
    }
    
    GiveDefine(tree, &answer, define);
    
    DestructorList(&answer);

    return NO_ERROR;
}

Error InteractionCompare(Tree* tree, char* define1, char* define2)
{
    Iterator answer1 = {};
    Iterator answer2 = {};

    fprintf(stderr, "%s\n", define1);
    fprintf(stderr, "%s\n", define2);

    ConstructorList(&answer1);
    ConstructorList(&answer2);


    Error error = ComposePath(tree, tree->root, &answer1, define1);
    printf("%d\n", answer1.list->num_elem);
    if (answer1.list->num_elem == -1)
    {
        DestructorList(&answer1);
        DestructorList(&answer2);
        return ELEMENT_NOT_FOUND;
    }
    error = ComposePath(tree, tree->root, &answer2, define2);
    printf("%d\n", answer2.list->num_elem);
    if (answer1.list->num_elem == -1)
    {
        DestructorList(&answer1);
        DestructorList(&answer2);
        return ELEMENT_NOT_FOUND;
    }

    GiveCompare(tree, &answer1, &answer2, define1, define2);

    DestructorList(&answer1);
    DestructorList(&answer2);

    return NO_ERROR;
}

Error GiveCompare(Tree* tree, Iterator* answer1, Iterator* answer2, char* define1, char* define2)
{
    int value1;
    int value2;

    Node* current = tree->root;
    printf("%s и %s - ", define1, define2);
    //size_t num = answer->list->num_elem;
    
    value1 = Pop_Front(answer1);
    value2 = Pop_Front(answer2);
    if (value1 == value2)
        printf("%s и %s -", define1, define2);
    while (value1 == value2)
    {
        if (value1 == 1)
        {
            printf(" %s", current->str.data);
            
            if (current->left != NULL)
                current = current->left;
        }
        else if (value1 == 0)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        value1 = Pop_Front(answer1);
        value2 = Pop_Front(answer2);
    }
    Node* current1 = current;
    Node* current2 = current;
    printf(" но %s", define1);
    size_t num1 = answer1->list->num_elem + 1;
    do
    {
        if (value1 == 1)
        {
            printf(" %s", current1->str.data);
            current1 = current1->left;
        }
        else if (value1 == 0)
        {
            printf(" не %s", current1->str.data);
            current1 = current1->right;
        }
        if (num1 != 0)
            printf(",");
        value1 = Pop_Front(answer1);
        num1--;
    } while (num1 != 0);

    printf(" а %s", define2);
    size_t num2 = answer2->list->num_elem + 1;
    do
    {
        if (value2 == 1)
        {
            printf(" %s", current2->str.data);
            current2 = current2->left;
        }
        else if (value2 == 0)
        {
            printf(" не %s", current2->str.data);
            current2 = current2->right;
        }
        if (num2 != 0)
            printf(",");
        value2 = Pop_Front(answer2);
        num2--;
    } while (num2 != 0);

    printf(".\n");

    return NO_ERROR;
}

Error GuessObject(Tree* tree, Node** node)
{
    char  answer[MAX_SIZE_ARG] = {};

    printf("Это %s?\n", (*node)->str.data);
    scanf("%s", answer);

    if (strcmp(answer, "да") == 0)
    {
        if ((*node)->left == NULL)
        {
            printf("Похоже, я угадала!\n");
            return EXIT;
        }
        Error error = GuessObject(tree, &(*node)->left);
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

            InputNodeData(&(*node)->left);
            
            (*node)->right = NewNode();
            if (((*node)->right) == NULL) {return ERROR_ALLOCATION;}

            (*node)->right->str.data = strdup(((*node)->str).data);

            printf("Чем %s отличается от %s\n", (*node)->left->str.data, (*node)->str.data);            

            InputNodeData(node);

            printf("Хорошо, я это запомню\n");
        
            tree->size++;

            Error error = GuessObject(tree, &(tree->root));
            if (error != NO_ERROR) {return error;}
        }
        Error error = GuessObject(tree, &(*node)->right);
        if (error != NO_ERROR) {return error;}
    }
    else 
    {
        printf("Введите корректно\n");
        Error error = GuessObject(tree, node);
        if (error != NO_ERROR) {return error;}
    }
    return NO_ERROR;
}

Error GiveDefine(Tree* tree, Iterator* answer, char* define)
{
    int value;
    Node* current = tree->root;
    printf("%s - ", define);
    size_t num = answer->list->num_elem;
    while(num != 0)
    {
        num--;
        value = Pop_Front(answer);
        if (value == 1)
        {
            printf(" %s", current->str.data);
            current = current->left;
        }
        else if (value == 0)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        if (num != 0)
            printf(",");
    }

    printf(".\n");
    return NO_ERROR;
}

Error ComposePath(Tree* tree, Node* node, Iterator* answer, char define[])
{
    if (define == NULL) {return DEFINE_IS_NULL;}

    if (node->left != NULL)
    {
        Push_Back(answer, TRUE);
        Error error = ComposePath(tree, node->left, answer, define);
        if (error != NO_ERROR) {return error;}
    }

    if (node->right != NULL)
    {
        Push_Back(answer, FASLE);
        Error error = ComposePath(tree, node->right, answer, define);
        if (error != NO_ERROR) {return error;}
    }

    printf("<%s> and <%s>\n", define, node->str.data);
    if (strcmp(node->str.data, define) == 0 )
    {
        printf("РАВНО\n");
        return EXIT;
    }
    else
        Pop_Back(answer);
    
    return NO_ERROR;
}

Error InputNodeData(Node** node)
{
    size_t arg = 0;
    
    char* source;
    char ref[MAX_SIZE_ARG];

    free((*node)->str.data);
    getline(&source, &arg, stdin);
    memcpy(ref, source, strlen(source) - 1);
    (*node)->str.data = strdup(ref);
    free(source);

    return NO_ERROR;
}