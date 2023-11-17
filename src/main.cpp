
#include "akinator.h"

int main()
{
    Tree tree = {};
    TreeError error = MainMenu(&tree);
    if (error != NO_ERROR)
    {
        DumpErrors(error);
        return 1;
    }
    return 0;
}