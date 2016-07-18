
#ifndef B_TREE_H_INCLUDED
#define B_TREE_H_INCLUDED

#include <stdint.h>

//#define __ONLY_RUS
#define __ONLY_ENG
//#define __ONLY_NUM

#ifdef __ONLY_RUS
    #define START_SYMBOL 'а'
    #define STOP_SYMBOL 'я'
#elif defined(__ONLY_ENG)
    #define START_SYMBOL 'a'
    #define STOP_SYMBOL 'z'
#elif defined(__ONLY_NUM)
    #define START_SYMBOL '0'
    #define STOP_SYMbol '9'
#else
    #define START_SYMBOL 1
    #define STOP_SYMBOL 254
#endif

#define MASK_SYMBOL '*'
/* B-tree data structure */
struct Node
{
    char value;
    unsigned char isEnd;
    struct Node* link[STOP_SYMBOL - START_SYMBOL + 1];
    struct Node* parent;
    uint16_t maxStringLength;
};
/* create B-tree node with value */
struct Node* BtreeInitNode (unsigned char value, struct Node* parent);
/* destroy one node WARNING!!! likely a memory leak*/
void         BTreeDestroyNode (struct Node* this);
/* destroy node and all their links, this method are secure */
void         BTreeDestroyBranch (struct Node* this);
/* insert byte string into tree */
void         BTreeInsertString (struct Node* root, const char* str);
/* print structure as tree */
void         BTreePrintTree (struct Node* this, int level, FILE* outputStream);
/* print data from structure */
void         BTreePrintListString (struct Node* this, FILE* outputStream);
/* find byte string in structure, return 0 if string is not correct */
int          BTreeFindString (struct Node* this, const char* str);
/* find prefix of string -||-, return 0 if string is not correct */
int          BTreeFindPrefix (struct Node* this, const char* str);
/* find variants to fill mask_symbol in str, return byte string */
char*        BTreeFillGap (struct Node* this, const char* str);
/* remove string from b-tree */
void         BtreeRemoveString (struct Node* this, const char* str);

/* print library compilation info */
void         printLibraryInfo ();

#endif

