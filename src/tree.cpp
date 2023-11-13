#include "tree.h"
#include "color.h"
#include "Dotter.h"


const char* outputfile = "testfile.txt";
const char* GRAPH_DOT  = "Graph.dot";

Error Constructor(Control* tree)
{
    assert(tree);
    
    
    tree->root = New();
    if (tree->root == NULL) {return ERROR_4;}

    memcpy(tree->root->data, "неизвестно кто", strlen("неизвестно кто"));
    tree->size = 1;
    //InputNodeData((tree->root), "неизвестно кто");

    return NO_ERROR;
}

Error GuessObject(Control* tree, Node** node)
{
    char answer[MAX_SIZE_ARG] = {};
    char*  str = {};
    char*  str2 = {};    
    
    printf("Это %s?\n", (*node)->data);
    scanf("%s", answer);

    if (strcmp(answer, "да") == 0)
    {
        if ((*node)->left == NULL)
        {
            printf("Похоже, я угадала!\n");
            return NO_ERROR;
        }
        Error error = GuessObject(tree, &(*node)->left);
        if (error != NO_ERROR)
            return error;
    }
    else if (strcmp(answer, "нет") == 0)
    {
        if ((*node)->right == NULL)
        {
            printf("К сожалению я не знаю, что вы загадали, скажите что это...\n");

            while (getchar() != '\n')   // clean buffer
                ;
            
            (*node)->left = New();

            if (((*node)->left) == NULL) {return ERROR_4;}

            InputNodeData(&(*node)->left, str);
            

            (*node)->right = New();
            if (((*node)->right) == NULL) {return ERROR_4;}

            memcpy((*node)->right->data, (*node)->data, MAX_SIZE_ARG);
    
            printf("Чем %s отличается от %s\n", (*node)->left->data, (*node)->data);            

            InputNodeData(node, str2);
        
            printf("Хорошо, я это запомню\n");
        
            tree->size++;

            Error error = GuessObject(tree, &(tree->root));
            if (error != NO_ERROR)
                return error;
        }
        Error error = GuessObject(tree, &(*node)->right);
        if (error != NO_ERROR)
                return error;
    }
    else 
    {
        printf("Введите корректно\n");
        Error error = GuessObject(tree, node);
        if (error != NO_ERROR)
                return error;
    }
    return NO_ERROR;
}

Error InputNodeData(Node** node, char* data)
{
    size_t arg = 0;
    
    memset((*node)->data, 0, MAX_SIZE_ARG);

    getline(&data, &arg, stdin);
    memcpy((*node)->data, data, strlen(data) - 1);
    free(data);

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
    node->left  =  NULL;
    node->right =  NULL;
    node->data  = (char*) calloc(64, sizeof(char));

    return node;
}

Error PrintNode(Node* node, FILE* To, Order order_value)
{
    if (!node) {fprintf(To, "nil "); return NO_ERROR;}

    fprintf(To, "( ");

    if (order_value == PRE_ORDER)
        fprintf(To, "%s ", node->data);
                   //%d
    Error error = PrintNode(node-> left, To, order_value);
    if (error != NO_ERROR)
        return error;

    
    if (order_value == IN_ORDER)
        fprintf(To, "%s ", node->data);
                   //%d
    error = PrintNode(node->right, To, order_value);
    if (error != NO_ERROR)
        return error;
    
    if (order_value == POST_ORDER)
        fprintf(To, "%s ", node->data);
                   //%d
    fprintf(To,") ");

    return NO_ERROR;
}

void Destructor(Control* tree)
{
    Delete(tree->root);
    tree->size = 99999;
}

void Delete(Node* node)
{
    if (!node) return;
    free(node->data);
    Delete(node-> left);
    Delete(node->right);

    free(node);
    return;
}

Error ReadTree(Node** node, FILE* From, Order order_value)
{
    char str[MAX_SIZE_ARG] = {};
    
    fscanf(From, "%s", str);

    if (strcmp(str, "nil") == 0)
        return NO_ERROR;

    if (strcmp(str, "(") == 0)
    {
        *node = New();
        if (*node == NULL) {return ERROR_4;}

        if (order_value == PRE_ORDER)
        {
            // fscanf(From, "%d", &arg);
            // (*node)->data = arg;
            fscanf(From, "%s", str);
            memcpy((*node)->data, str, MAX_SIZE_ARG);
        }
        
        Error error = ReadTree(&(*node)->left, From, order_value);
        if (error != NO_ERROR)
            return error;

        if (order_value == IN_ORDER)
        {
            // fscanf(From, "%d", &arg);
            // (*node)->data = arg;
            fscanf(From, "%s", str);
            memcpy((*node)->data, str, MAX_SIZE_ARG);
        }

        error = ReadTree(&(*node)->right, From, order_value);
        if (error != NO_ERROR)
            return error;
        
        if (order_value == POST_ORDER)
        {
            // fscanf(From, "%d", &arg);
            // (*node)->data = arg;
            fscanf(From, "%s", str);
            memcpy((*node)->data, str, MAX_SIZE_ARG);
        }

        fscanf(From, "%s", str);
        if (strcmp(str, ")") != 0)
        {
            return ERROR_1;
        }
    }
    else    
        return ERROR_2;

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
    if (2 * tree->size > MAX_SIZE_TREE) {return ERROR_5;}   

    Node* addresses[MAX_SIZE_TREE] = {};
    //              ^_______ 2 tree.size
    NodeTraversal(tree->root, addresses, 0);

    Qsort(addresses, 0, MAX_SIZE_TREE - 1);
    for (size_t i = 0; i < MAX_SIZE_TREE - 2; i++)
    {
        if ((addresses[i] == addresses[i + 1]) && (addresses[i] != NULL))
        {
            return ERROR_3; // loop
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

    
    dtNode((int) counter, node->data);

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
        case 0 << 0:
            return;
            break;
        case ERROR_1:
            printf("error1: has not been detected ')'\n");
            break;
        case ERROR_2:
            printf("error2: has not been detected '('\n");
            break;
        case ERROR_3:
            printf("error3: loop in tree\n");
            break;
        case ERROR_4:
            printf("error4: memory allocation is fail\n");
            break;
        case ERROR_5:
            printf("error5: address array has no memory for all addresses "
            "(change the constant responsible for the maximum tree size) \n");
            break;
        case ERROR_6:
            printf("error6: \n");
            break;
        default:
            printf("extra error\n");
            break;
    }
}