#include "akinator.h"
#include "graphic.h"

const char* lib = "lib.txt";


TreeError MainMenu(Tree* tree)
{
    //TreeError error;
    int answer = 0;

    printf(GREEN_COLOR "ИГРАТЬ"    RED_COLOR "[0] " 
           GREEN_COLOR "НАСТРОЙКИ" RED_COLOR "[1] " 
           GREEN_COLOR "ВЫХОД"     RED_COLOR "[2]\n" END_COLOR);
    

    if (scanf("%d", &answer) != 1)
    {
        CleanBuffer();
        return MainMenu(tree);
    }
    
    if (answer == 0)
    {
        TreeError error = Akinator(tree);
        if (error != NO_ERROR)
        {
            DumpErrors(error);
            DestructorTree(tree);
            return error;
        }
        
        return MainMenu(tree);
    }
    else if (answer == 1)
    {
        TreeError error = Parameters(tree);
        if (error != NO_ERROR)
        {
            DestructorTree(tree);
            DumpErrors(error);
            return error;
        }

        MainMenu(tree);
    }   
    else if (answer == 2)
    {
        DestructorTree(tree);
        return NO_ERROR;
    }
    else
        return MainMenu(tree);

    return NO_ERROR;
}

TreeError Akinator(Tree* tree)
{
    if (tree->root == NULL)
    {
        printf("Выберете библиотеку!\n");
        return MainMenu(tree);
    }

    Answer answer = GetAnswer(1);
    TreeError error;
    
    char define1[MAX_SIZE_ARG] = {};
    char define2[MAX_SIZE_ARG] = {};
    char* temp1;
    char* temp2;
    
    if (answer == GUESS)
    {
        error = InteractionGuess(tree);
    }
    else if (answer == DEFINE)
    {
        printf("Введите объект, определение которого вы хотите узнать\n");
        temp1 = ReadPhrase(stdin);
        error = InteractionDefine(tree, temp1);
        free(temp1);
    }
    else if (answer == COMPARE)
    {
        printf("Введите объекты, сравнить которые вы хотите\n");
        temp1 = ReadPhrase(stdin);
        temp2 = ReadPhrase(stdin);
    
        error = InteractionCompare(tree, define1, define2);
    
        free(temp2);
        free(temp1);
    }

    return error;
}

TreeError Parameters(Tree* tree)
{
    Answer answer = GetAnswer(2);
    TreeError error;

    if (answer == BASE)
        error = SelectBase(tree);
    else if (answer == GRAPHIC)
        error = DrawTree(tree);

    return error;
}

TreeError DrawTree(Tree* tree)
{
    if (tree->root == NULL)
    {
        printf("Сначала выберети базу, которую хотите увидеть\n");
        return MainMenu(tree);
    }
    else
    {
        GraphicDump(tree);
    }
    return NO_ERROR;
}

TreeError SelectBase(Tree* tree)
{
    if (tree->root != NULL)
        DestructorTree(tree);
    
    DIR *pDir;
    struct dirent *pDirent;
    TreeError error = NO_ERROR;

    pDir = opendir("database");
    printf("Выберете базу данных, с которой хотите работать\n");

    while ((pDirent = readdir(pDir)) != NULL) 
    {
        if (pDirent->d_type != 4) //type directory in dirent.h 
            printf("[%s]\n", pDirent->d_name);
    }
    closedir(pDir);

    char name_base[MAX_SIZE_NAME] = {};
    scanf("%s", name_base);

    int check = 0;

    pDir = opendir("database");

    while ((pDirent = readdir(pDir)) != NULL) 
    {
        if (strcmp(name_base, pDirent->d_name) == 0)
        {
            check++;
        }
    }
    closedir (pDir);

    if (check == 1)
    {
        char str[MAX_SIZE_NAME + strlen("database/")] = {};
        sprintf(str, "database/%s", name_base);

        FILE*   File = fopen(str, "r");
        if (File == NULL)
            return FILE_NOT_OPEN;

        ReadTree(tree, &tree->root, File, PRE_ORDER);
        fclose(File);
    }
    else
    {
        printf("Нет базы с таким именем\n");
        return SelectBase(tree);
    }
    
    return error;
}
Answer GetAnswer(int x)
{
    int answer = 0;
    if (x == 1)
    {
        printf(GREEN_COLOR  "Угадывание[1] " 
               YELLOW_COLOR "Определение[2] " 
               BLUE_COLOR   "Сравнение[3]\n" END_COLOR);
        if (scanf("%d", &answer) != 1)
        {
            printf("Введите число\n");
            GetAnswer(x);
        }
    }
    else if (x == 2)
    {
        printf(YELLOW_COLOR "ПОКАЗАТЬ ДЕРЕВО[4] "
               BLUE_COLOR   "ВЫБРАТЬ БАЗУ[5]\n" END_COLOR);
        if (scanf("%d", &answer) != 1)
        {
            CleanBuffer();
            return GetAnswer(x);
        }
    }

    CleanBuffer();

    if (answer > ENUM_ANSWER_SIZE) 
    {
        printf("Введите корректное число\n");
        GetAnswer(x);
    }
    else
        return (Answer) answer;
    
    return (Answer) 0;
}

TreeError InteractionGuess(Tree* tree)
{
    int  value =  INT_MAX;
    size_t start_size = tree->size;                     
    TreeError error = GuessObject(tree, &((*tree).root));
    if ((error != NO_ERROR) && (error != EXIT))
    {
        DestructorTree(tree);
        DumpErrors(error);
        return error;
    }
    
    if (start_size != tree->size)
    {
        printf("Хотите вы записать в библиотеку Ваши добавленные объекты? [1] - да [0] - нет\n");
        if ((scanf("%d", &value) == 1) && (value == 1))
        {
            FILE* File = fopen(lib, "w");
            if (File == NULL)
                return FILE_NOT_OPEN;

            PrintNode(tree->root, File, PRE_ORDER);
            fclose(File);
        }
    }

    return NO_ERROR;
}

TreeError InteractionDefine(Tree* tree, char* define)
{
    Iterator answer = {};
    
    ConstructorList(&answer);

    TreeError error = ComposePath(tree, tree->root, &answer, define);
    if (error == EXIT) {error = NO_ERROR;}
    
    if (answer.list->num_elem == 0)
    {
        DestructorList(&answer);
        return ELEMENT_NOT_FOUND;
    }
    if (error != NO_ERROR)
    {
        DestructorTree(tree);
        return error;
    }
    
    GiveDefine(tree, &answer, define);
    
    DestructorList(&answer);

    return NO_ERROR;
}

TreeError InteractionCompare(Tree* tree, char* define1, char* define2)
{
    Iterator answer1 = {};
    Iterator answer2 = {};

    ConstructorList(&answer1);
    ConstructorList(&answer2);


    TreeError error = ComposePath(tree, tree->root, &answer1, define1);
    if (error == EXIT) {error = NO_ERROR;}
    if (answer1.list->num_elem == 0)
    {
        DestructorList(&answer1);
        DestructorList(&answer2);
        return ELEMENT_NOT_FOUND;
    }
    if (error != NO_ERROR)
    {
        DestructorTree(tree);
        DumpErrors(error);
        return error;
    }
    error = ComposePath(tree, tree->root, &answer2, define2);
    if (error == EXIT) {error = NO_ERROR;}
    if (answer2.list->num_elem == 0)
    {
        DestructorList(&answer1);
        DestructorList(&answer2);
        return ELEMENT_NOT_FOUND;
    }
    if (error != NO_ERROR)
    {
        DestructorTree(tree);
        DumpErrors(error);
        return error;
    }

    GiveCompare(tree, &answer1, &answer2, define1, define2);

    DestructorList(&answer1);
    DestructorList(&answer2);

    return NO_ERROR;
}

TreeError GiveDefine(Tree* tree, Iterator* answer, char* define)
{
    int value;
    Node* current = tree->root;

    printf("%s -", define);
    
    while(answer->list->num_elem != 0)
    {
        value = (int) Pop_Front(answer);
        if (value == (int)true)
        {
            printf(" %s", current->str.data);
            current = current->left;
        }
        else if (value == (int)false)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        if (answer->list->num_elem != 0)
            printf(",");
    }

    printf(".\n");
    return NO_ERROR;
}

TreeError GiveCompare(Tree* tree, Iterator* answer1, Iterator* answer2, char* define1, char* define2)
{
    int value1;
    int value2;

    Node* current = tree->root;
    
    value1 = (int) Pop_Front(answer1);
    value2 = (int) Pop_Front(answer2);
    if (value1 == value2)
        printf("%s и %s -", define1, define2);
    while (value1 == value2)
    {
        if (value1 == (int)true)
        {
            printf(" %s", current->str.data);
            
            if (current->left != NULL)
                current = current->left;
        }
        else if (value1 == (int)false)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        value1 = (int) Pop_Front(answer1);
        value2 = (int) Pop_Front(answer2);
        printf(",");
    }

    Node* current1 = current;
    Node* current2 = current;
    printf(" но %s", define1);
    
    GiveSign(current1, answer1, value1);

    printf(",");

    printf(" а %s", define2);
    
    GiveSign(current2, answer2, value2);

    printf(".\n");

    return NO_ERROR;
}

void GiveSign(Node* current, Iterator* answer, int value)
{
    do
    {
        if (value == (int)true)
        {
            printf(" %s", current->str.data);
            current = current->left;
        }
        else if (value == (int)false)
        {
            printf(" не %s", current->str.data);
            current = current->right;
        }
        
        value = (int) Pop_Front(answer);
        if (answer->list->num_elem + 1 != 0)
            printf(",");
    } while (answer->list->num_elem + 1 != 0);
}

TreeError GuessObject(Tree* tree, Node** node)
{
    char  answer[MAX_SIZE_ARG] = {};

    printf("Это %s?\n", (*node)->str.data);
    scanf("%s", answer);
    TreeError error;

    if (strcmp(answer, "да") == 0)
    {
        if ((*node)->left == NULL)
        {
            printf("Похоже, я угадала!\n");
            return NO_ERROR;
        }
        error = GuessObject(tree, &(*node)->left);
    }
    else if (strcmp(answer, "нет") == 0)
    {
        if ((*node)->right == NULL)
        {
            printf("К сожалению я не знаю, что вы загадали, скажите что это...\n");

            CleanBuffer();

            (*node)->left = NewNode();

            if (((*node)->left) == NULL) {return ERROR_ALLOCATION;}
            
            free((*node)->left->str.data);
            (*node)->left->str.data = ReadPhrase(stdin);
            
            (*node)->right = NewNode();
            if (((*node)->right) == NULL) {return ERROR_ALLOCATION;}

            (*node)->right->str.data = strdup(((*node)->str).data);

            printf("Чем %s отличается от %s\n", (*node)->left->str.data, (*node)->str.data);            

            free((*node)->str.data);
            (*node)->str.data = ReadPhrase(stdin);

            printf("Хорошо, я это запомню\n");
        
            tree->size++;

            error = GuessObject(tree, &(tree->root));
        }
        error = GuessObject(tree, &(*node)->right);
    }
    else 
    {
        printf("Введите корректно\n");
        error = GuessObject(tree, node);
    }
    return error;
}

TreeError ComposePath(Tree* tree, Node* node, Iterator* answer, char define[])
{
    if (define == NULL) {return DEFINE_IS_NULL;}
    TreeError error;
    if (node->left != NULL)
    {
        Push_Back(answer, (int)true);
        error = ComposePath(tree, node->left, answer, define);
        if (error != NO_ERROR) {return error;}
    }

    if (node->right != NULL)
    {
        Push_Back(answer, (int)false);
        error = ComposePath(tree, node->right, answer, define);
        if (error != NO_ERROR) {return error;}
    }

    
    if (strcmp(node->str.data, define) == 0)
        return EXIT;
    else
        if (answer->list->num_elem != 0) {Pop_Back(answer);}
    
    return NO_ERROR;
}

char* ReadPhrase(FILE* File)
{
    size_t arg = 0;
    char* source = NULL;

    getline(&source, &arg, File);
    size_t lenght = strlen(source);
    source[lenght] = '\0';
    source[lenght - 1] = '\0';


    return source;
}

void CleanBuffer()
{
    while (getchar() != '\n')
        ;
    
}