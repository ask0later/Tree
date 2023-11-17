#include "tree.h"

TreeError ConstructorTree(Tree* tree)
{
    assert(tree);
    
    tree->root = NewNode();
    if (tree->root == NULL) {return ERROR_ALLOCATION;}

    char source[] = "неизвестно кто";

    TreeError error = StringConstructor(source, &((tree->root)->str));
    if (error != NO_ERROR) {return error;}

    tree->size = 1;

    return NO_ERROR;
}

TreeError StringConstructor(char* source, String* str)
{
    str->size = strlen(source) + 1;
    str->data = (char*) calloc(str->size, sizeof(char));
    if (str->data == NULL) {return ERROR_ALLOCATION;}
    memcpy(str->data, source, str->size);
    return NO_ERROR;
}

TreeError StringDestructor(String* str)
{
    str->size = (size_t)INT_MAX;
    free(str->data);
    str->data = NULL;
    return NO_ERROR;
}

// Error InsertValue(tree* tree, Elem_t value)
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

Node* NewNode()
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left     =  NULL;
    node->right    =  NULL;
    node->str.data =  NULL;
    node->str.size =     0;

    return node;
}

TreeError PrintNode(Node* node, FILE* To, Order order_value)
{
    if (To == NULL) {return FILE_NOT_OPEN;}

    if (!node) {fprintf(To, "nil "); return NO_ERROR;}

    fprintf(To, "( ");

    if (order_value == PRE_ORDER)
        fprintf(To, "%s ", node->str.data);
                   //%d
    TreeError error = PrintNode(node-> left, To, order_value);
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

void DestructorTree(Tree* tree)
{
    tree->size = 99999;
    
    DeleteNode(tree->root);
}

void DeleteNode(Node* node)
{
    if (!node) return;

    DeleteNode(node->left);
    DeleteNode(node->right);
    
    free((node->str).data);
    
    free(node);
    return;
}

TreeError ReadTree(Tree* tree, Node** node, FILE* From, Order order_value)
{
    if (From == NULL) {return FILE_NOT_OPEN;}
    char ptr[MAX_SIZE_ARG] = {};
    char* source;
    source = ptr;
    
    if (fscanf(From, "%s", ptr) == EOF) {return LIB_IS_EMPTY;}
    
    if (strcmp(source, "nil") == 0)
        return NO_ERROR;

    if (strcmp(source, "(") == 0)
    {
        *node = NewNode();
        tree->size++;

        if (*node == NULL) {return ERROR_ALLOCATION;}

        if (order_value == PRE_ORDER)
        {
            ReadTextPhrase(source, From);
            (*node)->str.data = strdup(source);
        }
        
        TreeError error = ReadTree(tree, &(*node)->left, From, order_value);
        if (error != NO_ERROR)
            return error;

        if (order_value == IN_ORDER)
        {
            ReadTextPhrase(source, From);
            (*node)->str.data = strdup(source);
        }

        error = ReadTree(tree, &(*node)->right, From, order_value);
        if (error != NO_ERROR)
            return error;
        
        if (order_value == POST_ORDER)
        {
            ReadTextPhrase(source, From);
            (*node)->str.data = strdup(source);
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

TreeError ReadTextPhrase(char* source, FILE* From)
{
    if (From == NULL) {return FILE_NOT_OPEN;}
    char c;
    size_t counter = 0;
    c = (char) getc(From);
    c = (char) getc(From);
    
    while ((c != 'n') && (c != '('))
    {
        source[counter] = c;
        
        c = (char) getc(From);
        counter++;
    }
    source[counter - 1] = 0;
    long position = ftell(From);

    if (position == 1L) {return ERROR_POSITIONING_FUNC;}

    fseek(From, position - 1, SEEK_SET);
    return NO_ERROR;
}


TreeError CheckNoLoop(Tree tree)
{
    printf("|||%d||||\n", tree.size);
    if (2 * tree.size > MAX_SIZE_TREE) {return ERROR_CONST;}   

    Node* addresses[MAX_SIZE_TREE] = {};
    //              ^_______ 2 tree.size
    NodeTraversal(tree.root, addresses, 0);

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


TreeError Qsort(Node* addresses[], int first, int last)
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
        
        Qsort(addresses, first, right);
        Qsort(addresses, left, last);
    }

    return NO_ERROR;
}

TreeError Swap(Node* addresses[], int left, int right)
{
    Node* temp = 0;
    temp = addresses[left];
    addresses[left] = addresses[right];
    addresses[right] = temp;

    return NO_ERROR;
}


TreeError NodeTraversal(Node* node, Node* addresses[], size_t counter)
{
    if (!node) {return NO_ERROR;}
    addresses[counter] = node;
    
    NodeTraversal(node->left , addresses, 2 * counter + 1);
    NodeTraversal(node->right, addresses, 2 * counter + 2);

    return NO_ERROR;
}

void TextDump(Tree* tree)
{
    PrintNode(tree->root, stdout, PRE_ORDER);
    printf("\n");
}


void DumpErrors(TreeError error)
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
        case LIB_IS_EMPTY:
            printf("error: libary is empty\n");
            break;
        case ELEMENT_NOT_FOUND:
            printf("error: object is not in the tree\n");
            break;
        default:
            printf("extra error\n");
            break;
    }
}