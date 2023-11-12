#include "tree.h"
#include "color.h"
#include "Dotter.h"


const char* outputfile = "testfile.txt";
const char* GRAPH_DOT  = "Graph.dot";

Error Constructor(Control* tree)
{
    assert(tree);
    tree->size = 0;

    return NO_ERROR;
}

Error InsertValue(Control* tree, Elem_t value)
{
    Node** current = &tree->root;

    for (size_t counter = 0; counter < tree->size + 1; counter++)
    {
        if (*current == NULL)
        {
            *current = New();
            (*current)->data = value; 
            tree->size++;
            return NO_ERROR;
        }
        
        if      (value <= (*current)->data)
            current = &(*current)->left;
        else if (value >  (*current)->data)
            current = &(*current)->right;    
    }
    return NO_ERROR;
}

Node* New()
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}
    node->left  =  NULL;
    node->right =  NULL;

    return node;
}

Error PrintNode(Node* node, FILE* To, Order order_value)
{
    if (!node) {fprintf(To, "nil "); return NO_ERROR;}

    fprintf(To, "( ");

    if (order_value == PRE_ORDER)
        fprintf(To, "%d ", node->data);

    Error error = PrintNode(node-> left, To, order_value);
    if (error != NO_ERROR)
        return error;

    
    if (order_value == IN_ORDER)
        fprintf(To, "%d ", node->data);
    
    error = PrintNode(node->right, To, order_value);
    if (error != NO_ERROR)
        return error;
    
    if (order_value == POST_ORDER)
        fprintf(To, "%d ", node->data);
    
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
    Delete(node-> left);
    Delete(node->right);
    
    free(node);
    return;
}

Error ReadTree(Node** node, FILE* From, Order order_value)
{
    
    char str[100] = {};
    int  arg      =  0;

    fscanf(From, "%s", str);

    if (strcmp(str, "nil") == 0)
        return NO_ERROR;

    if (strcmp(str, "(") == 0)
    {
        *node = New();
        if (*node == NULL) {return ERROR_4;}

        if (order_value == PRE_ORDER)
        {
            fscanf(From, "%d", &arg);
            (*node)->data = arg;
        }
        
        Error error = ReadTree(&(*node)->left, From, order_value);
        if (error != NO_ERROR)
            return error;

        if (order_value == IN_ORDER)
        {
            fscanf(From, "%d", &arg);
            (*node)->data = arg;
        }

        error = ReadTree(&(*node)->right, From, order_value);
        if (error != NO_ERROR)
            return error;
        
        if (order_value == POST_ORDER)
        {
            fscanf(From, "%d", &arg);
            (*node)->data = arg;
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

    char str[10] = {};
    sprintf(str, "%d", node->data);
    dtNode((int) counter, str);

    if (node->left  != 0)
    {
        GraphicDumpNode(node->left, counter * 2 + 1);
        dtLink((int) counter, (int)(counter * 2 + 1), "");
    }

    if (node->right != 0)
    {
        GraphicDumpNode(node->right, counter * 2 + 2);
        dtLink((int) counter, (int)(counter * 2 + 2), "");
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