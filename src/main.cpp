#include "tree.h"
#include "akinator.h"

//#include "readfile.h"
const char*  input =  "inputfile.txt";
const char*  lib   =  "lib.txt";

int main()
{
    struct Control tree = {};
    FILE* File = fopen(lib, "r");

    //Constructor(&tree);

    ReadTree(&tree.root, File, PRE_ORDER);

    Error error = GuessObject(&tree, &tree.root);
    
    if ((error != NO_ERROR) && (error != EXIT))
    {
        Destructor(&tree);
        DumpErrors(error);
        return 1;
    }
    
    PrintNode(tree.root, stderr, PRE_ORDER);
    PrintNode(tree.root, File, PRE_ORDER);
    GraphicDump(&tree);
    Destructor(&tree);

    fclose(File);
    
    return 0;
}




    // Interaction();
    // // FILE* From = fopen(input, "r");
    // FILE*   To = fopen(output, "r");

    // //Constructor(&tree);
    
    // ReadTree(&tree.root, To, PRE_ORDER);
    
    // Error error = GuessObject(&tree, &tree.root);
    
    // if ((error != NO_ERROR) && (error != EXIT))
    // {
    //     Destructor(&tree);
    //     DumpErrors(error);
    //     return 1;
    // }
    
    // PrintNode(tree.root, stderr, PRE_ORDER);
    // PrintNode(tree.root, To, PRE_ORDER);
    // GraphicDump(&tree);
    // Destructor(&tree);

    // fclose(To);
    // // fclose(From);



    // Error error = ReadTree(&tree.root, From, PRE_ORDER);
    // if (error != NO_ERROR)
    // {
    //     DumpErrors(error);
    //     Destructor(&tree);
    //     return 1;
    // }
    // // tree.root->right->left = tree.root->left->left;
    // error = CheckNoLoop(&tree);
    // if (error != NO_ERROR)
    // {
    //     DumpErrors(error);
    //     exit(1);
    // }
    // PrintNode(tree.root, stdout, PRE_ORDER);


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