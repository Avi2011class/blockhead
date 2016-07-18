#include "insertion-control_utf-8.h"
#include <stdio.h>
#include <stdlib.h>

void __swapData(void* __1, void* __2, size_t size)
{
    if (__1 == __2)
        return;
    size_t i;
    uint8_t* __pointer_1 = (uint8_t*)__1;
    uint8_t* __pointer_2 = (uint8_t*)__2;
    for (i = 0; i < size; i++, __pointer_1++, __pointer_2++)
    {
        *__pointer_1 ^= *__pointer_2;
        *__pointer_2 ^= *__pointer_1;
        *__pointer_1 ^= *__pointer_2;
    }
}

struct VectorOfInsertion* initVectorOfInsertion()
{
    struct VectorOfInsertion* new =
                (struct VectorOfInsertion*) calloc (1, sizeof (struct VectorOfInsertion));
    new->allocationSize = 4;
    new->usedSize = 0;
    new->data = (struct Insertion*) calloc (4, sizeof (struct Insertion));
    return new;
};

void destroyVectorOfINsertion (struct VectorOfInsertion* this)
{
    free (this->data);
    free (this);
}

void pushInsertion (struct VectorOfInsertion* this, int location, wchar_t symbol, wchar_t* result)
{
    if (this->usedSize == this->allocationSize)
    {
        this->data = (struct Insertion*) realloc (this->data,
                            sizeof (struct Insertion) * 2 * this->allocationSize);
        this->allocationSize *= 2;
    }
    this->data[this->usedSize].location = location;
    this->data[this->usedSize].symbol = symbol;
    wcsncpy ((this->data[this->usedSize].result), result, 49);
    this->usedSize ++;
}

void dumpVectorOfInsertion (struct VectorOfInsertion* this, FILE* outputStream)
{
    int i;
    fwprintf(outputStream, L"dump VectorOfInsertion, usedSize = %d, allocationSize = %d\n",
                this->usedSize, this->allocationSize);
    for (i = 0; i < this->usedSize; i++)
        fwprintf(outputStream, L"  %3d: location = %3d, symbol = %lc, result = %ls\n",
                 i, this->data[i].location, this->data[i].symbol, this->data[i].result);
}

void shuffleVectorOfInsertion (struct VectorOfInsertion* this)
{
    if (this->usedSize == 0)
        return;
    int i, j;
    for (i = 0; i < this->usedSize; i++)
    {
        j = rand() % (this->usedSize);
        __swapData((void*)(&(this->data[i])), (void*)(&(this->data[j])),
                   sizeof (struct Insertion));
    }
}
