#include "tree.h"
#include "color.h"
#include "Dotter.h"
#include "stack.h"

const char* outputfile = "testfile.txt";
const char* GRAPH_DOT  = "Graph.dot";

Error Constructor(Control* tree)
{
    assert(tree);
    
    tree->root = New();
    if (tree->root == NULL) {return ERROR_ALLOCATION;}

    char source[] = "неизвестно кто";

    Error error = StringConstructor(source, &((tree->root)->str));
    if (error != NO_ERROR) {return error;}

    tree->size = 1;

    return NO_ERROR;
}

Error StringConstructor(char* source, String* str)
{
    str->size = strlen(source) + 1;
    str->data = (char*) calloc(str->size, sizeof(char));
    if (str->data == NULL) {return ERROR_ALLOCATION;}
    memcpy(str->data, source, str->size);
    return NO_ERROR;
}

Error StringDestructor(String* str)
{
    str->size = (size_t)INT_MAX;
    free(str->data);
    str->data = NULL;
    return NO_ERROR;
}

Error GuessObject(Control* tree, Node** node)
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

            (*node)->left = New();

            if (((*node)->left) == NULL) {return ERROR_ALLOCATION;}

            InputNodeData(&(*node)->left);
            
            (*node)->right = New();
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

Error DefineObject(Control* tree, Node* node, Stack answer, char define[])
{
    if (define == NULL) {return DEFINE_IS_NULL;}
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

// Error InsertValue(Control* tree, Elem_t value)
// {
//     Node** current = &tree->root;

//     for (size_t counter = 0; counter < tree->size + 1; counter++)
//     {
//         if (*current == NULL)
//         {
//             *current = New();
//             (*current)->data = value; 
//             tree->size++;
//             return NO_ERROR;
//         }
        
//         if      (value <= (*current)->data)
//             current = &(*current)->left;
//         else if (value >  (*current)->data)
//             current = &(*current)->right;    
//     }
//     return NO_ERROR;
// }

Node* New()
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left     =  NULL;
    node->right    =  NULL;
    node->str.data =  NULL;
    node->str.size =     0;

    return node;
}

Error PrintNode(Node* node, FILE* To, Order order_value)
{
    if (To == NULL) {return FILE_NOT_OPEN;}

    if (!node) {fprintf(To, "nil "); return NO_ERROR;}

    fprintf(To, "( ");

    if (order_value == PRE_ORDER)
        fprintf(To, "%s ", node->str.data);
                   //%d
    Error error = PrintNode(node-> left, To, order_value);
    if (error != NO_ERROR)
        return error;

    
    if (order_value == IN_ORDER)
        fprintf(To, "%s ", node->str.data);
                   //%d
    error = PrintNode(node->right, To, order_value);
    if (error != NO_ERROR)
        return error;
    
    if (order_value == POST_ORDER)
        fprintf(To, "%s ", node->str.data);
                   //%d
    fprintf(To,") ");

    return NO_ERROR;
}

void Destructor(Control* tree)
{
    tree->size = 99999;
    
    Delete(tree->root);
}

void Delete(Node* node)
{
    if (!node) return;

    StringDestructor(&(node->str));
    Delete(node->left);
    
    Delete(node->right);

    free((node->str).data);
    free(node);
    return;
}

Error ReadTree(Node** node, FILE* From, Order order_value)
{
    if (From == NULL) {return FILE_NOT_OPEN;}
    char ptr[MAX_SIZE_ARG] = {};
    char* source;
    source = ptr;
    // [] ???
    
    fscanf(From, "%s", ptr);

    if (strcmp(source, "nil") == 0)
        return NO_ERROR;

    if (strcmp(source, "(") == 0)
    {
        *node = New();
        if (*node == NULL) {return ERROR_ALLOCATION;}

        if (order_value == PRE_ORDER)
        {
            ReadPhrase(source, From);
            StringConstructor(source, &(*node)->str);
        }
        
        Error error = ReadTree(&(*node)->left, From, order_value);
        if (error != NO_ERROR)
            return error;

        if (order_value == IN_ORDER)
        {
            //fscanf(From, "%s", source);
            ReadPhrase(source, From);
            StringConstructor(source, &(*node)->str);
        }

        error = ReadTree(&(*node)->right, From, order_value);
        if (error != NO_ERROR)
            return error;
        
        if (order_value == POST_ORDER)
        {
            ReadPhrase(source, From);
            StringConstructor(source, &(*node)->str);
        }

        fscanf(From, "%s", source);
        if (strcmp(source, ")") != 0)
        {
            return ERROR_RIGHT_BRACKET;
        }
    }
    else    
        return ERROR_LEFT_BRACKET;

    return NO_ERROR;
}

Error ReadPhrase(char* source, FILE* From)
{
    if (From == NULL) {return FILE_NOT_OPEN;}
    char c;
    size_t counter = 0;
    c = getc(From);
    while ((c != 'n') && (c != '('))
    {
        source[counter] = c;
        c = getc(From);
        counter++;
    }
    long position = ftell(From);
    if (position == 1L) {return ERROR_POSITIONING_FUNC;}

    fseek(From, position - 1, SEEK_SET);
    return NO_ERROR;
}
Error GraphicDump(Control* tree)
{
    dtBegin(GRAPH_DOT);                        // Начало dot-описания графа

    dtNodeStyle ().shape        ("ellipse");
    //dtNodeStyle ().style         ("filled");
    //dtNodeStyle ().fontcolor      ("black");

    GraphicDumpNode(tree->root, 1);

    dtEnd();                                   // Конец dot-описания графа

    dtRender(GRAPH_DOT);

    return NO_ERROR;
}

Error CheckNoLoop(Control* tree)
{
    // 
    if (2 * tree->size > MAX_SIZE_TREE) {return ERROR_CONST;}   

    Node* addresses[MAX_SIZE_TREE] = {};
    //              ^_______ 2 tree.size
    NodeTraversal(tree->root, addresses, 0);

    Qsort(addresses, 0, MAX_SIZE_TREE - 1);
    for (size_t i = 0; i < MAX_SIZE_TREE - 2; i++)
    {
        if ((addresses[i] == addresses[i + 1]) && (addresses[i] != NULL))
        {
            return ERROR_LOOP; // loop
        }
    }
    return NO_ERROR;
}


Error Qsort(Node* addresses[], int first, int last)
{
    if (first < last)
    {
        int left = first, right = last;
        Node* middle = addresses[(left + right) / 2];
        do
        {
            while (addresses[left] < middle)
                left++;
            while (addresses[right] > middle)
                right--;
            if (left <= right)
            {
                Swap(addresses, left, right);
                left++;
                right--;
            }
        } while (left <= right);
        //printf("l = %d, r = %d\n", left, right);
        Qsort(addresses, first, right);
        Qsort(addresses, left, last);
    }

    return NO_ERROR;
}

Error Swap(Node* addresses[], int left, int right)
{
    Node* temp = 0;
    temp = addresses[left];
    addresses[left] = addresses[right];
    addresses[right] = temp;

    return NO_ERROR;
}


Error NodeTraversal(Node* node, Node* addresses[], size_t counter)
{
    if (!node) {return NO_ERROR;}
    addresses[counter] = node;
    
    NodeTraversal(node->left , addresses, 2 * counter + 1);
    NodeTraversal(node->right, addresses, 2 * counter + 2);

    return NO_ERROR;
}

Error GraphicDumpNode(Node* node, size_t counter)
{
    if (!node) {return NO_ERROR;}

    
    dtNode((int) counter, node->str.data);

    if (node->left  != 0)
    {
        GraphicDumpNode(node->left, counter * 2 + 1);
        dtLink((int) counter, (int)(counter * 2 + 1), "да");
    }

    if (node->right != 0)
    {
        GraphicDumpNode(node->right, counter * 2 + 2);
        dtLink((int) counter, (int)(counter * 2 + 2), "нет");
    }

    return NO_ERROR;
}


void TextDump(Control* tree)
{
    PrintNode(tree->root, stdout, PRE_ORDER);
    printf("\n");
}


void DumpErrors(Error error)
{
    switch(error)
    {
        case NO_ERROR:
            return;
            break;
        case ERROR_RIGHT_BRACKET:
            printf("error: has not been detected ')'\n");
            break;
        case ERROR_LEFT_BRACKET:
            printf("error: has not been detected '('\n");
            break;
        case ERROR_LOOP:
            printf("error: loop in tree\n");
            break;
        case ERROR_ALLOCATION:
            printf("error: memory allocation is fail\n");
            break;
        case ERROR_CONST:
            printf("error: address array has no memory for all addresses "
            "(change the constant responsible for the maximum tree size) \n");
            break;
        case FILE_NOT_OPEN:
            printf("error: failed to open file \n");
            break;
        case ERROR_POSITIONING_FUNC:
            printf("error: error in file positioning functions\n");
            break;
        case DEFINE_IS_NULL:
            printf("error: define is null\n");
            break;
        default:
            printf("extra error\n");
            break;
    }
}