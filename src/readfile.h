#ifndef READlib
#define READlib

#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <assert.h>
#include <climits>

struct Text
{
    size_t  sizebuf;
    char*    buffer;
};

void     CreateBuffer(Text* buf, const char* From);
void     DeleteBuffer(Text* buf);
void         ReadFile(Text* buf, const char* From);

size_t  GetSizeFile(const char* From);
#endif