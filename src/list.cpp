#include "list.h"

//#define CHECH_MEMORY


void ConstructorList(Iterator* it)
{
    assert(it);

    it->index = 0;
    it->list  = (List*) calloc(1, sizeof(List));

    List* list = it->list;

    list->free     = 1;
    list->errors   = 0;
    list->num_elem = 0;

    list->size = NUM_NODES;

    list->nodes = (NodeList*) calloc(list->size + FICT_ELEM, sizeof(NodeList));

    FillMemory(list, FICT_ELEM, list->size + FICT_ELEM);

    list->nodes[0].data = POISON;
    list->nodes[0].prev = 0;
    list->nodes[0].next = 0;
}

void DestructorList(Iterator* it)
{
    List* list = it->list; 
    list->free   = POISON;
    list->errors = POISON;
    
    free(it->list->nodes);
    free(it->list);
}
 
ssize_t ListInsert(Iterator* it, int value)
{
    // position is index of element, which was after entered value
    // if (position == next[0])  value enter in start
    // if (prev[position] == -1) error!!!
    // if (position == 0)        value enter in end

    if (FindErrors(it) != 0)
        return -1;

    List*   list     = it ->  list;
    ssize_t position = it -> index;
    
#ifdef CHECH_MEMORY
    if (CheckMemory(it) != 0)
        return -1;
#endif

    ssize_t position_entered_elem = list->free;
    ssize_t free_next = list->nodes[list->free].next;           // free_next = next[free]

    list->nodes[list->free].data = value;                       // data[free] = value
    list->nodes[list->free].next = position;                    // next[free] = position
    list->nodes[list->free].prev = list->nodes[position].prev;           // prev[free] = prev[position]

    list->nodes[list->nodes[position].prev].next = list->free;  // next[prev[position]] = free
    list->nodes[position].prev = list->free;                    //      prev[position]  = free

    list->free = free_next;                                     // free = free_next

    list->num_elem++;
    
    return position_entered_elem;
}

ssize_t ListErase(Iterator* it)
{
    // int index = it.index
    
    //position - index of element, which need to remove
    // if (position == next[0])  value enter in start
    // if (position == prev[0])  value enter in end
    // if (prev[position] == -1) error!!!
    
    if (FindErrors(it) != 0)
        return -1;

    List* list   = it-> list;
    ssize_t position = it->index;

    ssize_t return_value = list->nodes[position].data; // return value

    list->nodes[list->nodes[position].prev].next = list->nodes[position].next;   // next[prev[position]] = next[position];
    list->nodes[list->nodes[position].next].prev = list->nodes[position].prev;   // prev[next[position]] = prev[position];

    list->nodes[position].data =          POISON;               // data[position] = POISON;
    list->nodes[position].prev =  POISON_COUNTER;               // prev[position] =     -1;
    list->nodes[position].next =      list->free;               // next[position] =   free;

    list->free = position;                                      // free = position;

    list->num_elem--;

    #ifdef CHECH_MEMORY
    if (CheckMemory(it) != 0)
        return -1;
    #endif

    return return_value;
}

void TextDumpList(Iterator* it)
{
    assert(it);

    List* list = it-> list;

    
    printf(GREEN_COLOR  "\nList information about this list\n");
    
    printf(BLUE_COLOR   "Head of list(next[0]) = %3ld.\n"
                        "Tail of list[prev[0]] = %3ld.\n",\
                        list->nodes[0].next,\
                        list->nodes[0].prev);

    printf(YELLOW_COLOR "First free element = %3ld.\n", list->free);
    printf("______________________________________\n");
    
    printf(BOLD_RED_COLOR "INDX ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        printf("%3lu ", counter);
    }
    printf("\n");

    printf(BOLD_RED_COLOR "DATA ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR " ЯД ");
        else if ((list->nodes[counter].data == POISON) && (list->nodes[counter].prev == -1))
            printf(YELLOW_COLOR " ЯД ");
        else if (list->nodes[counter].data == POISON)
            printf(GREEN_COLOR " ЯД ");
        else
            printf(GREEN_COLOR "%3d ",list->nodes[counter].data);
    }
    printf("\n");
    
    printf(BOLD_RED_COLOR "NEXT ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%3ld ", list->nodes[counter].next);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%3ld ", list->nodes[counter].next);
        else
            printf(GREEN_COLOR "%3ld ",list->nodes[counter].next);
    }
    printf("\n");
    printf(BOLD_RED_COLOR "PREV ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%3ld ", list->nodes[counter].prev);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%3ld ", list->nodes[counter].prev);
        else
            printf(GREEN_COLOR "%3ld ",list->nodes[counter].prev);
    }
    printf("\n");
    printf(YELLOW_COLOR "______________________________________\n");
    printf(GREEN_COLOR "End information\n\n");
    printf(END_COLOR);
}

ssize_t CheckMemory(Iterator* it)
{
    List* list = it-> list;

    if (list->free == (int) list->size)
        return IncreaseRealloc(it);
    else if ((list->num_elem <= list->size / 4) && (list->size > MIN_SIZE_LIST)) 
        return  ReduceRealloc(it);
    return 0;
}
ssize_t IncreaseRealloc(Iterator* it)
{
    List* list = it-> list;

    list->size *= VALUE_REALLOC;
    NodeList* new_list = (NodeList*) realloc(list->nodes, (list->size + FICT_ELEM) * sizeof(NodeList));
    if (new_list == NULL) {list->errors = 1 << 9; return list->errors;}
    list->nodes = new_list;
        
    FillMemory(list, list->size / VALUE_REALLOC + FICT_ELEM, list->size + FICT_ELEM);
    
    list->errors = FindErrors(it);
    return list->errors;
}
ssize_t ReduceRealloc(Iterator* it)
{
    it->list->size /= VALUE_REALLOC;
    
    Linearization(it);
    
    return it->list->errors;
}

ssize_t FindErrors(Iterator* it)
{
    if (it          == NULL)                           return 1 << 0;
    
    List* list = it-> list;

    if (list        == NULL)                           return 1 << 1;
    if (list->nodes == NULL)                           return 1 << 2;

    if (it->index > (int) list->size)         list->errors |= 1 << 3;
    if (list->nodes[it->index].prev == -1)    list->errors |= 1 << 4;
    if (list->free > (int) list->size)        list->errors |= 1 << 5;
    if (list->nodes[0].data != POISON)        list->errors |= 1 << 6;
    
    list->errors |= СheckForLooping(list);
    list->errors |= LogicCheck(list);

    return list->errors;
}

ssize_t СheckForLooping(List* list)
{
    size_t counter_full = 0;
    size_t counter      = (size_t) list->nodes[0].next;
    size_t counterstart = counter;
    if (list->num_elem == 0)
        return 0;
    do
    {
        counter = (size_t) list->nodes[counter].next;
        counter_full++;
        if (counter == 0)
            return 0;
        if (counter == counterstart)
            return 1 << 7;
    } while (counter_full < list->size);
    
    return 0;
}

ssize_t LogicCheck(List* list)
{
    size_t counter_full = 0;
    size_t counter_new  = 0;
    size_t counter_old  = 0;
    do
    {
        counter_old = counter_new;
        counter_new = (size_t) list->nodes[counter_old].next;
        if (list->nodes[counter_new].prev != (int) counter_old)
            return 1 << 8;
        counter_full++;
    } while (counter_full < list->size);
    
    return 0;
}


int DumpErrors(Iterator* it)
{
    List* list = it-> list;
    if (list->errors == 0)
        list->errors = 1 << 0;

    switch(list->errors)
    {
        case 1 << 0:
            printf("ERRORS : it is null\n");
            break;
        case 1 << 1:
            printf("ERRORS : list is null\n");
            break;
        case 1 << 2:
            printf("ERRORS : nodes is null\n");
            break;
        case 1 << 3:
            printf("ERROR : index above size\n");
            break;
        case 1 << 4:
            printf("ERROR : index on free memory\n");
            break;
        case 1 << 5:
            printf("ERROR : excess memory is used\n");
            break;
        case 1 << 6:
            printf("ERROR : the dictitious element was affecred\n");
            break;
        case 1 << 7:
            printf("ERROR : elements are looped\n");
            break;
        case 1 << 8:
            printf("ERROR : logical error(prev[next[position]] != position)\n");
            break;
        case 1 << 9:
            printf("ERROR : FAIL ALLOCATION\n");
            break;
        default:
            printf("EXTRA ERROR\n");
            break;
    }

    return (int) list->errors;
}


ssize_t Push_Back(Iterator* it, int value)
{
    it->index = 0;
    
    return ListInsert(it, value);
}

ssize_t Push_Front(Iterator* it, int value)
{
    it->index = it->list->nodes[0].next;
    
    return ListInsert(it, value);
}

ssize_t Pop_Back(Iterator* it)
{
    it->index = it->list->nodes[0].prev;
    
    return ListErase(it);
}

ssize_t Pop_Front(Iterator* it)
{
    it->index = it->list->nodes[0].next;
    
    return ListErase(it);
}

ssize_t Begin(Iterator* it)
{
    if (FindErrors(it) != 0)
        return -1;

    return (it->list->nodes[0].next);
}

ssize_t   End(Iterator* it)
{
    if (FindErrors(it) != 0)
        return -1;
    // return 0 is OK
    return (it->list->nodes[it->list->nodes[0].prev].next);  // always return 0
}

int GetValueList(Iterator* it)
{
    if (FindErrors(it) != 0)
        return -1;

    int return_value = it->list->nodes[it->index].data;
    return return_value;
}

ssize_t SetValueList(Iterator* it, int value)
{
    if (FindErrors(it) != 0)
        return -1;

    it->list->nodes[it->index].data = value;
    return it->index;
}

ssize_t Next(Iterator* it)
{
    if (FindErrors(it) != 0)
        return -1;

    return (it->list->nodes[it->index].next);
}

void FillMemory(List* list, size_t start, size_t end)
{
    assert(list);

    for (size_t counter = start; counter < end; counter++) 
    {
        list->nodes[counter].data = POISON;
        list->nodes[counter].next = (int) (counter + FICT_ELEM);
        list->nodes[counter].prev = POISON_COUNTER;
    }
}

ssize_t FindElemByValue(Iterator* it, int value)
{
    // if (FindErrors(it) != 0)
    //     return -1;
    
    printf(GREEN_COLOR "                              __ATTENTION__\n"
           "was used to search for an element by value, which uses, oh my God, a loop\n");

    ssize_t counter      = 0;
    do
    {
        if (it->list->nodes[counter].data == value)
            return counter;
        counter = it->list->nodes[counter].next;
    } while (counter != 0);

    return 0;
}

ssize_t Linearization(Iterator* it)
{
    List* list = it->list;
    
    int temporaryList[MAX_SIZE_LIST] = {};
    size_t counter      = 0;
    size_t counter_full = 0;
    do
    {
        temporaryList[counter_full] = list->nodes[counter].data;
        counter = (size_t) list->nodes[counter].next;
        counter_full++;
    } while (counter != 0);

    free(list->nodes);

    NodeList* new_list = (NodeList*) calloc(list->size + FICT_ELEM, sizeof(NodeList)); 
    if (new_list == NULL) {list->errors = 1 << 9; return list->errors;}
    list->nodes = new_list;

    for (counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
    {
        list->nodes[counter].data = temporaryList[counter];

        if (counter == counter_full - 1)
            list->nodes[counter].next = 0;
        else
            list->nodes[counter].next = (int) (counter + 1);
        if (counter == 0)
            list->nodes[counter].prev = (int) (counter_full - 1);
        else 
            list->nodes[counter].prev = (int) (counter - 1);
    }
    list->free = (int) counter_full;

    FillMemory(list, counter_full, list->size + FICT_ELEM);

    list->errors = FindErrors(it);
    return list->errors;
}