
#include "database.h"
#include "b-tree.h"

struct StringDatabase* databaseInit()
{
    struct StringDatabase* newDatabase = (struct StringDatabase*) malloc (sizeof(struct StringDatabase));
    newDatabase->root = BtreeInitNode('>', NULL);
    return newDatabase;
}
void databaseDestroy(struct StringDatabase* this)
{
   BTreeDestroyBranch(this->root);
   free(this);
}
void databaseInsert(struct StringDatabase* this, const char* str)
{
    BTreeInsertString(this->root, str);
}
int databaseFindString(struct StringDatabase* this, const char* str)
{
    return BTreeFindString(this->root, str);
}
int databaseFindPrefix(struct StringDatabase* this, const char* str)
{
    return BTreeFindPrefix(this->root, str);
}
void databasePrint(struct StringDatabase* this, FILE* outpusStream)
{
    BTreePrintListString (this->root, outpusStream);
}
char* databaseFillGap (struct StringDatabase* this, const char* mask)
{
    return BTreeFillGap (this->root, mask);
}
void databaseRead(struct StringDatabase* this, FILE* inputStream)
{
	char* __buffer = (char*) malloc (sizeof (char) * 1000);
	while (fscanf (inputStream, "%1000s", __buffer) == 1)
	    databaseInsert (this, __buffer);
    free (__buffer);
}
void databaseRemove(struct StringDatabase* this, const char* str)
{
    BtreeRemoveString(this->root, str);
}

