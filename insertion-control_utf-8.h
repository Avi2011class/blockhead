#ifndef INSERTIONCONTROLUTF8HINCLUDED
#define INSERTIONCONTROLUTF8HINCLUDED
#include <stdint.h>
#include <wchar.h>

struct Insertion
{
    int location;
    wchar_t symbol;
    wchar_t result[55];
};


struct VectorOfInsertion
{
    struct Insertion* data;
    int usedSize;
    int allocationSize;
};

struct VectorOfInsertion* initVectorOfInsertion ();
void destroyVectorOfINsertion (struct VectorOfInsertion* this);
void pushInsertion (struct VectorOfInsertion* this, int location, wchar_t symbol, wchar_t* result);
void dumpVectorOfInsertion (struct VectorOfInsertion* this, FILE* outputStream);
void shuffleVectorOfInsertion (struct VectorOfInsertion* this);
#endif // INSERTION-CONTROL_UTF-8_H_INCLUDED
