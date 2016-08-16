#ifndef BTREEUTF8HINCLUDED
#define BTREEUTF8HINCLUDED

#include <stdint.h>

#define __ONLY_RUS
//#define __ONLY_ENG
//#define __ONLY_NUM

#if defined(__ONLY_RUS)
	#define START_SYMBOL ((wchar_t)L'а')
	#define STOP_SYMBOL ((wchar_t)L'я')
#elif defined(__ONLU_ENG)
	#define START_SYMBOL ((wchar_t)'a')
	#define STOP_SYMBOL ((wchar_t)'z')
#elif defined(__ONLU_NUM)
	#define START_SYMBOL ((wchar_t)'0')
	#define STOP_SYMBOL ((wchar_t)'9')
#else
	#define START_SYMBOL ((wchar_t)'a')
	#define STOP_SYMBOL ((wchar_t)'z')
#endif

/* B-tree data structure */
struct Node
{
    wchar_t value;
    uint8_t isEnd;
    struct Node* link[STOP_SYMBOL - START_SYMBOL + 1];
    struct Node* parent;
    uint16_t maxStringLength;
};
/* create B-tree node with value */
struct Node* BtreeInitNode (wchar_t value, struct Node* parent);
/* destroy one node WARNING!!! likely a memory leak*/
void         BTreeDestroyNode (struct Node* this);
/* destroy node and all their links, this method are secure */
void         BTreeDestroyBranch (struct Node* this);
/* insert byte string into tree */
void         BTreeInsertString (struct Node* root, const wchar_t* str);
/* print structure as tree */
void         BTreePrintTree (struct Node* this, int level, FILE* outputStream);
/* print data from structure */
void         BTreePrintListString (struct Node* this, FILE* outputStream);
/* find byte string in structure, return 0 if string is not correct */
int          BTreeFindString (struct Node* this, const wchar_t* str);
/* find prefix of string -||-, return 0 if string is not correct */
int          BTreeFindPrefix (struct Node* this, const wchar_t* str);
/* find variants to fill mask_symbol in str, return byte string */
void         BtreeRemoveString (struct Node* this, const wchar_t* str);

/* print library compilation info */
void         printLibraryInfo ();


#endif // B-TREE_UTF-8_H_INCLUDED
