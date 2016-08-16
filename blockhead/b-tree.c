
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <stddef.h>

#include "b-tree.h"

#define LINK_COUNT STOP_SYMBOL - START_SYMBOL + 1
#define for0(x) int __macroIterator; for(__macroIterator = 0; __macroIterator < x; __macroIterator++)
#define foruchar(__char, __start, __stop) unsigned char __char; for(__char = __start; __char <= __stop; __char++)
#define MAX(__1, __2) (((__1) > (__2)) ? (__1) : (__2))
#define MIN(__1, __2) (((__1) < (__2)) ? (__1) : (__2))

#if !defined(__ONLY_RUS) && !defined(__ONLY_ENG)
    #define FORMAT(CHAR) (CHAR)
#else
    #define FORMAT(CHAR) tolower(CHAR)
#endif

inline int __checkString (const char* __str, int __needMaskSymbol)
{
    int maskCount = 0;
    unsigned char* __charIterator;
    for(__charIterator = (unsigned char*)__str; *__charIterator; __charIterator++)
    {
        if(*__charIterator == MASK_SYMBOL)
            maskCount++;
        else
            if(*__charIterator < START_SYMBOL || *__charIterator > STOP_SYMBOL)
                return 0;
    }
    if((__needMaskSymbol && maskCount == 1) || (!__needMaskSymbol))
        return 1;
    return 0;
}

struct Node* BtreeInitNode (unsigned char value, struct Node* parent)
{
    struct Node* newNode = (struct Node*) calloc (1, sizeof(struct Node));
    newNode->value = FORMAT(value);
    newNode->isEnd = 0;
    newNode->parent = parent;
    newNode->maxStringLength = 0;
    return newNode;
};

void BTreeDestroyNode (struct Node* this)
{
    if(this)
        free(this);
}

void BTreeDestroyBranch (struct Node* this)
{
    if(this)
    {
        int i;
        for(i = 0; i < LINK_COUNT; i++)
            BTreeDestroyBranch (this->link[i]);
        BTreeDestroyNode (this);
    }
}

void BTreeInsertString (struct Node* root, const char* str)
{
    if(!__checkString(str, 0))
    {
        fprintf(stderr, "Incorrect insert string \"%s\"\n", str);
        return;
    }
    unsigned char* strPointer = (unsigned char*) str;
    int strLength = strlen (str);
    root->maxStringLength = MAX(root->maxStringLength, strLength);
    struct Node* currentNode = root;
    for0(strLength)
    {
        if(!currentNode->link[FORMAT(*strPointer) - START_SYMBOL])
            currentNode->link[FORMAT(*strPointer) - START_SYMBOL] =
                        BtreeInitNode(FORMAT(*strPointer), currentNode);
        currentNode = currentNode->link[FORMAT(*strPointer) - START_SYMBOL];
        strPointer++;
    }
    currentNode->isEnd = 1;
}

void BTreePrintTree (struct Node* this, int level, FILE* outputStream)
{
    if(!this)
        return;
    int i;
    for(i = 0; i < level; i++)
        fprintf(outputStream, "  ");
    fprintf (outputStream, "%c%c\n", this->value, (this->isEnd) ? '|' : ' ');
    for(i = 0; i < LINK_COUNT; i++)
        BTreePrintTree(this->link[i], level + 1, outputStream);
}
#define DECSENT_TREE(__TREE, __STR_POINTER, __NODE_ITERATOR, __ERROR_CODE) \
    int __str_length = strlen(__STR_POINTER); \
    unsigned char* __STR_ITERATOR = (unsigned char*)__STR_POINTER; \
    struct Node* __NODE_ITERATOR = __TREE; \
    for0(__str_length) \
    { \
        if(__NODE_ITERATOR->link[(*__STR_ITERATOR) - START_SYMBOL]) \
        { \
            __NODE_ITERATOR = __NODE_ITERATOR->link[(*__STR_ITERATOR) - START_SYMBOL]; \
            __STR_ITERATOR++; \
        } \
        else \
            return __ERROR_CODE; \
    } \

int BTreeFindPrefix (struct Node* this, const char* str)
{
    if(!__checkString (str, 0))
    {
        fprintf(stderr, "Incorrect prefix string \"%s\"\n", str);
        return 0;
    }
    DECSENT_TREE(this, str, currentNode, 0);
    return 1;
}

int BTreeFindString (struct Node* this, const char* str)
{
    if (!__checkString(str, 0))
    {
        fprintf(stderr, "Incorrect find string \"%s\"\n", str);
        return 0;
    }
    DECSENT_TREE(this, str, currentNode, 0);
    if(currentNode->isEnd)
        return 1;
    return 0;
}

char* BTreeFillGap (struct Node* this, const char* str)
{
    if (!__checkString(str, 1))
    {
        fprintf(stderr, "Incorrect mask string \"%s\"\n", str);
        char* resultStr = (char*) calloc (2, sizeof(char));
        return resultStr;
    }
    char* workString = (char*) calloc (sizeof(char), 1 + strlen(str));
    strcpy (workString, str);
    char* gap = strchr (workString, MASK_SYMBOL);
    if (gap == NULL)
        return NULL;
    char* resultStr = (char*) calloc (LINK_COUNT + 1, sizeof(char));
    foruchar(nextSymbol, START_SYMBOL, STOP_SYMBOL)
    {
        *gap = nextSymbol;
        if(BTreeFindString (this, workString))
            strncat (resultStr, (char*)&nextSymbol, 1);
    }
    return resultStr;
}

void __DFSPrint (struct Node* __this, char* __prefix, int __prefixSize, FILE* __outputStream)
{
    if(__this->isEnd)
        fprintf (__outputStream, "%s\n", __prefix);

    foruchar (nextSymbol, START_SYMBOL, STOP_SYMBOL)
        if(__this->link[nextSymbol - START_SYMBOL])
        {
            __prefix[__prefixSize] = nextSymbol;
            __DFSPrint (__this->link[nextSymbol - START_SYMBOL], __prefix, __prefixSize + 1, __outputStream);
            __prefix[__prefixSize] = 0;
        }
}

void BTreePrintListString (struct Node* this, FILE* outputStream)
{
    printf ("B-tree maxStringLength = %d\n", this->maxStringLength);
    char* buffer = (char*) calloc (this->maxStringLength + 5, sizeof(char));
    __DFSPrint (this, buffer, 0, outputStream);
    free (buffer);
}

void printLibraryInfo ()
{
    printf ("B-tree library info:\n");
    #if defined(_WIN32) || defined(_WIN64)
        printf("    OS Windows\n");
    #elif defined(__linux) || defined(linux)
        printf("    OS Linux\n");
    #endif // OS type
    printf ("    File name: %s\n", __FILE__);
    printf ("    Date: %s, time: %s\n", __DATE__, __TIME__);
    #if defined(__GNUC__)
        printf ("    GNU C compiler, version: %s\n", __VERSION__);
    #else
        printf("Unknown compiler\n");
    #endif // defined
}

struct Node* findEndNodeOfPrefix(struct Node* this, const char* str)
{
    if(!__checkString(str, 0))
    {
        fprintf(stderr, "Incorrect insert string \"%s\"\n", str);
        return NULL;
    }
    DECSENT_TREE(this, str, currentNode, NULL);
    return currentNode;
}
void BtreeRemoveString (struct Node* this, const char* str)
{
    if(!__checkString(str, 0))
    {
        fprintf(stderr, "Incorrect insert string \"%s\"\n", str);
        return ;
    }
    DECSENT_TREE(this, str, currentNode, );
    currentNode->isEnd = 0;
    struct Node* buffer;
    for(;;)
    {
        if (currentNode == this && currentNode->isEnd)
            return;

        unsigned char iter;
        uint8_t flag = 0;
        for (iter = START_SYMBOL; iter <= STOP_SYMBOL; iter++)
            if (currentNode->link[iter - START_SYMBOL])
                flag = 1;
        if (flag)
            break;
        else
        {
            char currentValue = currentNode->value;
            buffer = currentNode->parent;
            free (currentNode);
            currentNode = buffer;
            currentNode->link[currentValue - START_SYMBOL] = NULL;
        }
    }
    return;
}

