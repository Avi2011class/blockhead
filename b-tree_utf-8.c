
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


#include "b-tree_utf-8.h"

#define LINK_COUNT (STOP_SYMBOL - START_SYMBOL)

#if !defined(__ONLY_RUS) && !defined(__ONLY_ENG)
    #define FORMAT(CHAR) (CHAR)
#else
    #define FORMAT(CHAR) towlower(CHAR)
#endif

#define for0(x) int __macroIterator; for(__macroIterator = 0; __macroIterator < x; __macroIterator++)
#define MAX(__1, __2) (((__1) > (__2)) ? (__1) : (__2))
#define MIN(__1, __2) (((__1) < (__2)) ? (__1) : (__2))
#define forwchar(__char, __start, __stop) wchar_t __char; for(__char = __start; __char <= __stop; __char++)
inline int __checkString (const wchar_t* __str)
{
    wchar_t* __charIterator;
    for (__charIterator = (wchar_t*)__str; *__charIterator; __charIterator++)
    {
		if (FORMAT(*__charIterator) < START_SYMBOL || FORMAT(*__charIterator) > STOP_SYMBOL)
			return 0;
    }
    return 1;
}

struct Node* BtreeInitNode (wchar_t value, struct Node* parent)
{
    struct Node* newNode = (struct Node*) calloc (1, sizeof(struct Node));
    newNode->value = FORMAT((wint_t)value);
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
        for(i = 0; i <= LINK_COUNT; i++)
            BTreeDestroyBranch (this->link[i]);
        BTreeDestroyNode (this);
    }
}

void BTreeInsertString (struct Node* root, const wchar_t* str)
{
    if(!__checkString(str))
    {
        fwprintf(stderr, L"Incorrect insert string \"%ls\"\n", str);
        return;
    }
    wchar_t* strPointer = (wchar_t*) str;
    int strLength = wcslen (str);
    root->maxStringLength = MAX (root->maxStringLength, strLength);
    struct Node* currentNode = root;
    for0(strLength)
    {
        if(!currentNode->link[FORMAT (*strPointer) - START_SYMBOL])
            currentNode->link[FORMAT (*strPointer) - START_SYMBOL] =
                        BtreeInitNode(FORMAT (*strPointer), currentNode);
        currentNode = currentNode->link[FORMAT (*strPointer) - START_SYMBOL];
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
        fwprintf(outputStream, L"  ");
    fwprintf (outputStream, L"%lc%c\n", this->value, (this->isEnd) ? L'|' : L' ');
    for(i = 0; i < LINK_COUNT; i++)
        BTreePrintTree(this->link[i], level + 1, outputStream);
}

#define DECSENT_TREE(__TREE, __STR_POINTER, __NODE_ITERATOR, __ERROR_CODE) \
    int __str_length = wcslen(__STR_POINTER); \
    wchar_t* __STR_ITERATOR = (wchar_t*)__STR_POINTER; \
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

int BTreeFindPrefix (struct Node* this, const wchar_t* str)
{
    if(!__checkString (str))
    {
        fwprintf(stderr, L"Incorrect prefix string \"%ls\"\n", str);
        return 0;
    }
    DECSENT_TREE(this, str, currentNode, 0);
    return 1;
}

int BTreeFindString (struct Node* this, const wchar_t* str)
{
    if (!__checkString(str))
    {
        fwprintf(stderr, L"Incorrect find string \"%ls\"\n", str);
        return 0;
    }
    DECSENT_TREE(this, str, currentNode, 0);
    if(currentNode->isEnd)
        return 1;
    return 0;
}

void __DFSPrint (struct Node* __this, wchar_t* __prefix, int __prefixSize, FILE* __outputStream)
{
    if(__this->isEnd)
        fwprintf (__outputStream, L"%ls\n", __prefix);

    forwchar (nextSymbol, START_SYMBOL, STOP_SYMBOL)
        if(__this->link[nextSymbol - START_SYMBOL])
        {
            __prefix[__prefixSize] = nextSymbol;
            __DFSPrint (__this->link[nextSymbol - START_SYMBOL], __prefix, __prefixSize + 1, __outputStream);
            __prefix[__prefixSize] = 0;
        }
}

void BTreePrintListString (struct Node* this, FILE* outputStream)
{
    wprintf (L"B-tree maxStringLength = %d\n", this->maxStringLength);
    wchar_t* buffer = (wchar_t*) calloc (this->maxStringLength + 5, sizeof(wchar_t));
    __DFSPrint (this, buffer, 0, outputStream);
    free (buffer);
}

void BtreeRemoveString (struct Node* this, const wchar_t* str)
{
    if(!__checkString(str))
    {
        fwprintf(stderr, L"Incorrect insert string \"%ls\"\n", str);
        return ;
    }
    DECSENT_TREE(this, str, currentNode, );
    currentNode->isEnd = 0;
    struct Node* buffer;
    for(;;)
    {
        if (currentNode == this && currentNode->isEnd)
            return;

        uint8_t flag = 0;
        forwchar(iter, START_SYMBOL, STOP_SYMBOL)
            if (currentNode->link[iter - START_SYMBOL])
                flag = 1;
        if (flag)
            break;
        else
        {
            wchar_t currentValue = currentNode->value;
            buffer = currentNode->parent;
            free (currentNode);
            currentNode = buffer;
            currentNode->link[currentValue - START_SYMBOL] = NULL;
        }
    }
    return;
}

void __TEST_BTREE(void)
{
	setlocale (LC_ALL, "ru_RU.utf8");
	wprintf(L"start = %lc, stop = %lc\n", START_SYMBOL, STOP_SYMBOL);
	wprintf(L"check = %d\n", __checkString(L"топор"));
	struct Node* B = BtreeInitNode(L'>', NULL);
	BTreeInsertString(B, L"топорище");
	BTreeInsertString(B, L"топот");
	BTreeInsertString(B, L"говор");
	BTreeInsertString(B, L"гопота");
	BTreeInsertString(B, L"то123");
	BTreePrintTree(B, 0, stdout);
	BTreePrintListString(B, stdout);
	wprintf(L"findPrefix %ls = %d\n", L"топ", BTreeFindPrefix(B, L"топ"));
	wprintf(L"findPrefix %ls = %d\n", L"ток", BTreeFindPrefix(B, L"ток"));
	wprintf(L"findString %ls = %d\n", L"топорище", BTreeFindString(B, L"топор"));
	wprintf(L"findString %ls = %d\n", L"топ", BTreeFindString(B, L"топ"));
	BtreeRemoveString(B, L"топорище");
	BTreePrintListString(B, stdout);
	BTreeDestroyBranch(B);
	wprintf(L"\n\n");
}

