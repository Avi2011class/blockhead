#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "b-tree.h"
struct StringDatabase
{
    struct Node* root;
};

struct StringDatabase* databaseInit();
void databaseDestroy(struct StringDatabase* this);
void databaseInsert(struct StringDatabase* this, const char* str);
int databaseFindString(struct StringDatabase* this, const char* str);
int databaseFindPrefix(struct StringDatabase* this, const char* str);
void databasePrint(struct StringDatabase* this, FILE* outpusStream);
char* databaseFillGap(struct StringDatabase* this, const char* mask);
void databaseRead(struct StringDatabase* this, FILE* inputStream);
void databaseRemove(struct StringDatabase* this, const char* str);

#endif // DATABASE_H_INCLUDED
