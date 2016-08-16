#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "b-tree_utf-8.h"
struct StringDatabase
{
    struct Node* root;
};
struct StringDatabase* databaseInit();
void databaseDestroy(struct StringDatabase* this);
void databaseInsert(struct StringDatabase* this, const wchar_t* str);
int databaseFindString(struct StringDatabase* this, const wchar_t* str);
int databaseFindPrefix(struct StringDatabase* this, const wchar_t* str);
void databasePrint(struct StringDatabase* this, FILE* outpusStream);
void databaseRead(struct StringDatabase* this, FILE* inputStream);
void databaseRemove(struct StringDatabase* this, const wchar_t* str);

#endif // DATABASE_H_INCLUDED
