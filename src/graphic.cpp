#include "graphic.h"
#include "Dotter.h"


void GraphicDumpList(Iterator* it)
{
    List* list = it-> list;

    dtBegin("Graph.dot");             // Начало dot-описания графа

    CreateGraphicNodes(list);       // Создаем узлы
    CreateGraphicLinks(list);       // Создаем связи

    dtEnd(); // Конец dot-описания графа

    dtRender("Graph.dot", "Graph.jpg", "jpg", false);
}
void CreateGraphicNodes(List* list)
{
    assert(list);

    dtNodeStyle ().shape            ("box");   // Устанавливаем стиль узлов
    dtNodeStyle ().style         ("filled");
    dtNodeStyle ().fontcolor      ("black");

    char str[MAX_SIZE_LIST] = {};
    for (size_t counter = 0; counter < list->size; counter++)
    {
        if ((list->nodes[counter].prev == POISON_COUNTER) || (counter == 0)) 
        {
            sprintf(str, "INDX = %2lu\n"
                         "DATA = ЯД\n"
                         "NEXT = %2ld\n"
                         "PREV = %2ld", counter, list->nodes[counter].next, list->nodes[counter].prev);
        }
        else
        {
            sprintf(str, "INDX = %2lu\n"
                         "DATA = %2d\n"
                         "NEXT = %2ld\n"
                         "PREV = %2ld", counter, list->nodes[counter].data, list->nodes[counter].next, list->nodes[counter].prev);
        }
        if (list->nodes[counter].prev == POISON_COUNTER)     // Устанавливаем цвет узлов
            dtNodeStyle().fillcolor("#F8D568");//HEX_YELLOW       
        else if (counter == 0)
            dtNodeStyle().fillcolor("#EE204D");//HEX_RED
        else if ((counter == (size_t)list->nodes[0].prev) || (counter == (size_t)list->nodes[0].next))
            dtNodeStyle().fillcolor("#7BF2DA");//HEX_TIFFANY
        else 
            dtNodeStyle().fillcolor("#21C912");//HEX_GREEN

        dtNode((int) counter, str);
    }
}
void CreateGraphicLinks(List* list)
{
    assert(list);
    int next = 0;
    dtLinkStyle().style  ("invis"); // Устанавливаем стиль связей
    dtLinkStyle().color("#FFFFFF"); //HEX_WHITE
    for (size_t counter = 0; counter < list->size - 1; counter++)
    {
        dtLink ((int) counter, (int) (counter + 1), "");//"fontsize=200");
    }
    dtLinkStyle().style   ("bold");
    dtLinkStyle().color("#000000"); //HEX_BLACK
    
    for (size_t counter = 0; counter < list->size - 1; counter++)
    {
        next = (int) list->nodes[counter].next;
        dtLink((int)counter, next, "");
    }
}



TreeError GraphicDump(Tree* tree)
{
    dtBegin("Graph.dot");                        // Начало dot-описания графа

    dtNodeStyle ().shape        ("ellipse");
    //dtNodeStyle ().style         ("filled");
    //dtNodeStyle ().fontcolor      ("black");

    GraphicDumpNode(tree->root, 1);

    dtEnd();                                   // Конец dot-описания графа

    dtRender("Graph.dot", "Graph.jpg", "jpg", false);

    return NO_ERROR;
}


TreeError GraphicDumpNode(Node* node, size_t counter)
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