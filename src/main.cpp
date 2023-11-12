#include "tree.h"
//#include "readfile.h"
const char*  input =  "inputfile.txt";
const char* output = "outputfile.txt";

int main()
{
    struct Control tree = {};

    FILE* From = fopen(input, "r");
    //FILE*   To = fopen(output, "w");
    Constructor(&tree);


    Error error = ReadTree(&tree.root, From, PRE_ORDER);
    if (error != NO_ERROR)
    {
        DumpErrors(error);
        Destructor(&tree);
        return 1;
    }
    // tree.root->right->left = tree.root->left->left;
    error = CheckNoLoop(&tree);
    if (error != NO_ERROR)
    {
        DumpErrors(error);
        exit(1);
    }
    PrintNode(tree.root, stdout, PRE_ORDER);


    GraphicDump(&tree);
    Destructor(&tree);

    //fclose(To);
    fclose(From);
    return 0;
}


//     InsertValue(&tree, 20);

//     InsertValue(&tree, 15);

//     InsertValue(&tree, 40);

//     InsertValue(&tree, 5);


//  ReadTree(&tree.root, From, POST_ORDER);
    

    // Error error = PrintNode(tree.root, stdout, POST_ORDER);
    // if (error != NO_ERROR)
    // {
    //     DumpErrors(error);
    //     Destructor(&tree);
    // }