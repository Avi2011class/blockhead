#include "database_utf-8.h"
#include "b-tree_utf-8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
struct StringDatabase* databaseInit()
{
    struct StringDatabase* newDatabase = (struct StringDatabase*) malloc (sizeof(struct StringDatabase));
    newDatabase->root = BtreeInitNode(L'>', NULL);
    return newDatabase;
}
void databaseDestroy(struct StringDatabase* this)
{
   BTreeDestroyBranch(this->root);
   free(this);
}
void databaseInsert(struct StringDatabase* this, const wchar_t* str)
{
    BTreeInsertString(this->root, str);
}
int databaseFindString(struct StringDatabase* this, const wchar_t* str)
{
    return BTreeFindString(this->root, str);
}
int databaseFindPrefix(struct StringDatabase* this, const wchar_t* str)
{
    return BTreeFindPrefix(this->root, str);
}
void databasePrint(struct StringDatabase* this, FILE* outpusStream)
{
    BTreePrintListString (this->root, outpusStream);
}

void databaseRead(struct StringDatabase* this, FILE* inputStream)
{
	wchar_t* __buffer = (wchar_t*) malloc (sizeof (wchar_t) * 1000);
	while (fwscanf (inputStream, L"%1000ls", __buffer) == 1)
	    databaseInsert (this, __buffer);
    free (__buffer);
}
void databaseRemove(struct StringDatabase* this, const wchar_t* str)
{
    BtreeRemoveString(this->root, str);
}
void __TESTDATABASE()
{
    setlocale (LC_ALL, "ru_RU.utf8");
    struct StringDatabase* D = databaseInit();
    databasePrint(D, stdout);
    databaseInsert(D, L"привет");
    databaseInsert(D, L"приветик");
    databaseInsert(D, L"курица");
    databaseInsert(D, L"килька");
    databaseInsert(D, L"котик");
    databaseInsert(D, L"крот");
    databaseInsert(D, L"яблоко");
    databaseInsert(D, L"примочки");
    databasePrint(D, stdout);
    databaseRemove(D, L"хренотень");
    databaseRemove(D, L"крот");
    databaseRemove(D, L"яблоко");
    databaseRemove(D, L"примочки");
    databasePrint(D, stdout);
    databaseDestroy(D);
}
