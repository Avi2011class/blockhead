#include "computerNext_utf-8.h"
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
#include <limits.h>

#include "computerNext_utf-8.h"
#define MAX(__1, __2) (((__1) > (__2)) ? (__1) : (__2))
#define _GNU_SOURCE



void DFSMaxLen (wchar_t* field, char* visited, int iStart, int jStart, int height, int width,
                struct StringDatabase* base, wchar_t* DFSpath, uint8_t isFill, unsigned int* result);

void DFSstart (wchar_t* field, char* visited,
			int iStart, int jStart, int height, int width, unsigned int wordLength,
			struct StringDatabase* base, wchar_t* DFSpath, uint8_t isFill,
            int bufLocation, wchar_t bufInserted, struct VectorOfInsertion* memory);

//////////////////////////////////////////////////////////////////////////////////////////////////////
int computerNext (wchar_t* field, int height, int width,
				  struct StringDatabase* base,
				  wchar_t* result, int* location, wchar_t* inserted)
{
    if (isFieldFull(field, height, width))
    {
        *inserted = L'\0';
        *result = 0;
        *location = 0;
        return 0;
    }

	int i0, j0;
	unsigned int maxLen = 0;
	/* allocation the memory to control of visited cells */
	char* visited = (char*) calloc (height * width + 1, sizeof (char));

	/* allocation the memory for monitoring the way in which the search has come to this cell */
	wchar_t* DFSpath = (wchar_t*) calloc (height * width + 2, sizeof (wchar_t));

    struct VectorOfInsertion* memory = initVectorOfInsertion();

    for (i0 = 0; i0 < height; i0++)
        for (j0 = 0; j0 < width; j0++)
        {
            visited [i0 * height + j0] = 1;
            DFSMaxLen(field, visited, i0, j0, height, width, base, DFSpath, 0, &maxLen);
            memset (visited, 0, height * width);
        }
    //wprintf (L"--------maxLen=%u--------\n", maxLen);
    /* start DFS path for all the cells of the field */
	for (i0 = 0; i0 < height; i0++)
		for (j0 = 0; j0 < width; j0++)
		{
			/* mark as visited */
			visited [i0 * height + j0] = 1;
			/* run DFS and not check result */
			//DFSstart (field, visited, i0, j0, height, width, base, DFSpath, result, 0, location, 0, inserted, 0);
			DFSstart (field, visited, i0, j0, height, width, maxLen, base, DFSpath, 0, 0, 0, memory);
			/* mark as unvisited */
			memset (visited, 0, height * width);
		}
    if (memory->usedSize == 0)
    {
        *result = 0;
        *location = 0;
        *inserted = 0;
        free (DFSpath);
        free (visited);
        destroyVectorOfINsertion (memory);
        return 0;
    }
    shuffleVectorOfInsertion(memory);


    struct Insertion bestInsertion;
    unsigned int nextLen, bestLen = INT_MAX;

    int i;
    for (i = 0; i < memory->usedSize; i++)
    {
        nextLen = 0;
        field[memory->data[i].location] = memory->data[i].symbol;
        for (i0 = 0; i0 < height; i0++)
            for (j0 = 0; j0 < width; j0++)
            {
                visited [i0 * height + j0] = 1;
                DFSMaxLen(field, visited, i0, j0, height, width, base, DFSpath, 0, &nextLen);
                memset (visited, 0, height * width);
            }
        /////
        //wprintf(L"location=%d, symbol=%lc, str=%ls, nextLen=%u\n",
        //        memory->data[i].location, memory->data[i].symbol, memory->data[i].result, nextLen);
        /////
        if (nextLen == INT_MAX)
            nextLen = 0;
        if (nextLen < bestLen)
        {
            bestLen = nextLen;
            memcpy (&bestInsertion, &(memory->data[i]), sizeof (struct Insertion));
        }
        field[memory->data[i].location] = 0;
    }


    *location = bestInsertion.location;
    *inserted = bestInsertion.symbol;
    result = wcscpy (result, (wchar_t*)(&bestInsertion.result));
	/* free allocated memory */
	free (DFSpath);
	free (visited);
	destroyVectorOfINsertion (memory);

	/* return the length of the word of 0 if word has not been found */
	if (*result != 0)
		return wcslen (result);
	return 0;
}


/* return wcslen of max word */
void DFSMaxLen (wchar_t* field, char* visited, int iStart, int jStart, int height, int width,
                struct StringDatabase* base, wchar_t* DFSpath, uint8_t isFill, unsigned int* result)
{
    /* if search came to clear cell and it had fill some cell affer, it must be stoped */
	if (isFill && field[iStart * width + jStart] == 0)
		return;

	/* if search came to a call containing the letter it must to do next actions*/
	if (field[iStart * width + jStart] != 0)
	{
		/* If there is not the search path in the prefix base, search must stop */
		if (databaseFindPrefix(base, (wchar_t*) DFSpath) == 0)
			return;

		/* else go next, mark this cell as visited and add the letter from this cell to end of DFSpath */
		visited[ iStart * width + jStart ] = 1;
		unsigned int lenDFSpath = wcslen (DFSpath);
		DFSpath[lenDFSpath] = field[iStart * width + jStart];

		/* If the algorithm inserts the missing letters before, the way in which the search
		 * has come to this cell is a potential result */
		if (isFill)
			/* If the current word are longer than previous result, algorithm will replace last */
			if (databaseFindString (base, DFSpath))
				*result = MAX(lenDFSpath + 1, *result);

		/* This code part is a macro that checks the next path and calls the next search iteration */
#define GO_NEXT \
	if (iNext > -1 && iNext < height && jNext > -1 && jNext < width && /* check next cell location */ \
			visited[iNext * width + jNext] == 0) /* search can't go to visited cell */ \
		DFSMaxLen(field, visited, iNext, jNext, height, \
				 width, base, DFSpath, isFill, result);

		int iNext;
		int jNext;void shuffleVectorOfInsertion (struct VectorOfInsertion* this);

		/* to up */
		iNext = iStart;
		jNext = jStart;
		iNext--;
		GO_NEXT;

		/* to left */
		iNext = iStart;
		jNext = jStart;
		jNext--;
		GO_NEXT;

		/* to down */
		iNext = iStart;
		jNext = jStart;
		iNext++;
		GO_NEXT;

		/* to right */
		iNext = iStart;
		jNext = jStart;
		jNext++;
		GO_NEXT;
		/* Macro further useless */
#undef GO_NEXT

		/* and go BACK from the cell */
		DFSpath[lenDFSpath] = 0;
		visited[ iStart * width + jStart ] = 0;
	}

	/* if search came to clear cell and it had not fill some cell affer, it must insert some letter */
	if (field[iStart * width + jStart] == 0 && isFill == 0)
	{
		/* if search came to clear cell and it had fill some cell affer, it must be stoped */
		if (databaseFindPrefix(base, DFSpath) == 0)
			return;

		/* Loop through the added character */
		wchar_t symbol;
		for (symbol = START_SYMBOL; symbol <= STOP_SYMBOL; symbol++)
		{
			/* insert this letter */
			field[iStart * width + jStart] = symbol;

			/* and do previous operations */
			visited[ iStart * width + jStart ] = 1;
			unsigned int lenDFSpath = wcslen (DFSpath);
			DFSpath[lenDFSpath] = field[iStart * width + jStart];

			if (databaseFindString(base, DFSpath))
                *result = MAX(lenDFSpath + 1, *result);

#define GO_NEXT \
	if (iNext > -1 && iNext < height && jNext > -1 && jNext < width && \
			visited[iNext * width + jNext] == 0) \
		DFSMaxLen(field, visited, iNext, jNext, height, \
				 width, base, DFSpath, 1, result);


			int iNext;
			int jNext;

			iNext = iStart;
			jNext = jStart;
			iNext--;
			GO_NEXT;

			iNext = iStart;
			jNext = jStart;
			jNext--;
			GO_NEXT;

			iNext = iStart;
			jNext = jStart;
			iNext++;
			GO_NEXT;

			iNext = iStart;
			jNext = jStart;
			jNext++;
			GO_NEXT;
#undef GO_NEXT

			DFSpath[lenDFSpath] = 0;
			visited[ iStart * width + jStart ] = 0;
			field[iStart * width + jStart] = 0;
		}
	}



};


/* the main search method */
void DFSstart (wchar_t* field, char* visited,
			int iStart, int jStart, int height, int width, unsigned int wordLength,
			struct StringDatabase* base, wchar_t* 	DFSpath, uint8_t isFill,
            int bufLocation, wchar_t bufInserted, struct VectorOfInsertion* memory)
{
	/* if search came to clear cell and it had fill some cell affer, it must be stoped */
	if (isFill && field[iStart * width + jStart] == 0)
		return;

	/* if search came to a call containing the letter it must to do next actions*/
	if (field[iStart * width + jStart] != 0)
	{
		/* If there is not the search path in the prefix base, search must stop */
		if (databaseFindPrefix(base, (wchar_t*) DFSpath) == 0)
			return;

		/* else go next, mark this cell as visited and add the letter from this cell to end of DFSpath */
		visited[ iStart * width + jStart ] = 1;
		unsigned int lenDFSpath = wcslen (DFSpath);
		DFSpath[lenDFSpath] = field[iStart * width + jStart];

		/* If the algorithm inserts the missing letters before, the way in which the search
		 * has come to this cell is a potential result */
		if (isFill)
		{
			/* If the current word are longer than previous result, algorithm will replace last */
			if (databaseFindString(base, DFSpath) && lenDFSpath + 1 == wordLength)
			{
				pushInsertion(memory, bufLocation, bufInserted, DFSpath);
			}
		}
		/* This code part is a macro that checks the next path and calls the next search iteration */
#define GO_NEXT \
	if (iNext > -1 && iNext < height && jNext > -1 && jNext < width && /* check next cell location */ \
			visited[iNext * width + jNext] == 0) /* search can't go to visited cell */ \
		DFSstart(field, visited, iNext, jNext, height, \
				 width, wordLength, base, DFSpath, isFill, \
				  bufLocation, bufInserted, memory);  /* go next */

		int iNext;
		int jNext;

		/* to up */
		iNext = iStart;
		jNext = jStart;
		iNext--;
		GO_NEXT;

		/* to left */
		iNext = iStart;
		jNext = jStart;
		jNext--;
		GO_NEXT;

		/* to down */
		iNext = iStart;
		jNext = jStart;
		iNext++;
		GO_NEXT;

		/* to right */
		iNext = iStart;
		jNext = jStart;
		jNext++;
		GO_NEXT;
		/* Macro further useless */
#undef GO_NEXT

		/* and go BACK from the cell */
		DFSpath[lenDFSpath] = 0;
		visited[ iStart * width + jStart ] = 0;
	}

	/* if search came to clear cell and it had not fill some cell affer, it must insert some letter */
	if (field[iStart * width + jStart] == 0 && isFill == 0)
	{
		/* if search came to clear cell and it had fill some cell affer, it must be stoped */
		if (databaseFindPrefix(base, DFSpath) == 0)
			return;

		/* Loop through the added character */
		wchar_t symbol;
		for (symbol = START_SYMBOL; symbol <= STOP_SYMBOL; symbol++)
		{
			/* insert this letter */
			field[iStart * width + jStart] = symbol;

			/* and do previous operations */
			visited[ iStart * width + jStart ] = 1;
			unsigned int lenDFSpath = wcslen (DFSpath);
			DFSpath[lenDFSpath] = field[iStart * width + jStart];

			if (databaseFindString(base, DFSpath) && lenDFSpath + 1 == wordLength)
			{
			    pushInsertion(memory, iStart * width + jStart, symbol, DFSpath);
			}

#define GO_NEXT \
	if (iNext > -1 && iNext < height && jNext > -1 && jNext < width && \
			visited[iNext * width + jNext] == 0) \
		DFSstart(field, visited, iNext, jNext, height, \
				 width, wordLength, base, DFSpath, 1,\
				 iStart * width + jStart, symbol, memory);

			int iNext;
			int jNext;

			iNext = iStart;
			jNext = jStart;
			iNext--;
			GO_NEXT;

			iNext = iStart;
			jNext = jStart;
			jNext--;
			GO_NEXT;

			iNext = iStart;
			jNext = jStart;
			iNext++;
			GO_NEXT;

			iNext = iStart;
			jNext = jStart;
			jNext++;
			GO_NEXT;
#undef GO_NEXT

			DFSpath[lenDFSpath] = 0;
			visited[ iStart * width + jStart ] = 0;
			field[iStart * width + jStart] = 0;
		}
	}

}
uint8_t isFieldFull (wchar_t* field, int height, int width)
{
    int i;
    for (i = 0; i < (height * width); i++)
    {
        if (field[i] == L'\0')
            return 0;
    }
    return 1;
}
