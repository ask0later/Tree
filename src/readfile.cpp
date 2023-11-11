#include "readfile.h"


void CreateBuffer(Text* text, const char* From)
{
    text->sizebuf = GetSizeFile(From);

    text->buffer = (char*) calloc(text->sizebuf, sizeof(char));
    
    ReadFile(text, From);
}

void DeleteBuffer(Text* text)
{
    free(text->buffer);
    text->sizebuf = INT_MAX;
}

void ReadFile(Text* text, const char* From)
{
    FILE* fname = fopen(From, "r");

    fread(text->buffer, sizeof(char), text->sizebuf, fname);
    
    fclose(fname);
    
}

size_t GetSizeFile(const char* binary)
{
    struct stat buff = {};
    stat(binary, &buff);
    return (size_t) (buff.st_size + 1);
}