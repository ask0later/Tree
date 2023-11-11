#include "tree.h"
//#include "readfile.h"
const char* test = "testfile.txt";

int main()
{
    struct Control tree = {};

    FILE* From = fopen(test, "r");
    Constructor(&tree);


    InsertValue(&tree, 10);
    TextDump(&tree);

    InsertValue(&tree, 7);
    TextDump(&tree);

    InsertValue(&tree, 15);
    TextDump(&tree);

    InsertValue(&tree, 20);
    TextDump(&tree);


    // ReadTree(&tree.root, From, IN_ORDER);

    //PrintNode(tree.root, stdout, IN_ORDER);

    GraphicDump(&tree);
    Destructor(&tree);

    fclose(From);

    return 0;
}


    // InsertValue(&tree, 10);
    // TextDump(&tree);

    // InsertValue(&tree, 5);
    // TextDump(&tree);

    // InsertValue(&tree, 15);
    // TextDump(&tree);

    // InsertValue(&tree, 20);
    // TextDump(&tree);

    // InsertValue(&tree, 12);
    // TextDump(&tree);

    // InsertValue(&tree, 11);
    // TextDump(&tree);


    // WriteToFile(&tree);
    
    // GraphicDump(&tree);