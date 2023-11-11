#include "tree.h"
#include "color.h"
#include "Dotter.h"


const char* outputfile = "testfile.txt";
const char* GRAPH_DOT = "Graph.dot";

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
            printf("BBBB\n");
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

    PrintNode(node-> left, To, order_value);
    
    if (order_value == IN_ORDER)
        fprintf(To, "%d ", node->data);
    
    PrintNode(node->right, To, order_value);
    
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
    int arg = 0;
    fscanf(From, "%s", str);

    if (strcmp(str, "nil") == 0)
        return NO_ERROR;


    if (strcmp(str, "(") == 0)
    {
        *node = New();

        if (order_value == PRE_ORDER)
        {
            fscanf(From, "%d", &arg);
            (*node)->data = arg;
        }
        
        ReadTree(&(*node)->left, From, order_value);

        if (order_value == IN_ORDER)
        {
            fscanf(From, "%d", &arg);
            (*node)->data = arg;
        }

        ReadTree(&(*node)->right, From, order_value);
        
        if (order_value == PRE_ORDER)
        {
            fscanf(From, "%d", &arg);
            (*node)->data = arg;
        }

        fscanf(From, "%s", str);
        if (strcmp(str, ")") != 0)
            return ERROR_1;
    }
    
    return NO_ERROR;
}

Error GraphicDump(Control* tree)
{
    dtBegin(GRAPH_DOT);                        // Начало dot-описания графа

    dtNodeStyle ().shape        ("ellipse");
    dtNodeStyle ().style         ("filled");
    dtNodeStyle ().fontcolor      ("black");

    GraphicDumpNode(tree->root, 1);

    dtEnd();                                   // Конец dot-описания графа

    dtRender(GRAPH_DOT);

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


void WriteToFile(Control* tree)
{
    FILE* name = fopen(outputfile, "w");
    PrintNode(tree->root, name, PRE_ORDER);
    fclose(name);
}


void TextDump(Control* tree)
{
    PrintNode(tree->root, stdout, PRE_ORDER);
    printf("\n");
}