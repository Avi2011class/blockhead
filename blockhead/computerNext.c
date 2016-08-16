#include "computerNext.h"

int computerNext (char* field, int height, int width,
			struct StringDatabase* base,
			char* result, int* location, char* inserted)
{
	unsigned int i0, j0;
	/* allocation the memory to control of visited cells */
	char* visited = (char*) calloc (height * width + 1, sizeof (char));

	/* allocation the memory for monitoring the way in which the search has come to this cell */
	char* DFSpath = (char*) calloc (height * width + 2, sizeof (char));

	/* start DFS path for all the cells of the field */
	for (i0 = 0; i0 < height; i0++)
		for (j0 = 0; j0 < width; j0++)
		{
			/* mark as visited */
			visited [i0 * height + j0] = 1;
			/* run DFS and not check result */
			DFSstart (field, visited, i0, j0, height, width, base, DFSpath, result, 0, location, 0, inserted, 0);
			/* mark as unvisided */
			memset (visited, 0, height * width + 1);
		}

	/* free allocated memory */
	free (DFSpath);
	free (visited);

	/* return the length of the word of 0 if word has not been found */
	if (*result != 0)
		return strlen (result);
	return 0;
}
/* the main search method */
void DFSstart (char* field, char* visited, int iStart, int jStart, int height, int width,
			struct StringDatabase* base, char* DFSpath, char* result, uint8_t isFill,
			int* location, int bufLocation, char* inserted, char bufInserted)
{
	/* if search came to clear cell and it had fill some cell affer, it must be stoped */
	if (isFill && field[iStart * width + jStart] == 0)
		return;

	/* if search came to a call containing the letter it must to do next actions*/
	if (field[iStart * width + jStart] != 0)
	{
		/* If there is not the search path in the prefix base, search must stop */
		if (databaseFindPrefix(base, DFSpath) == 0)
			return;

		/* else go next, mark this cell as visited and add the letter from this cell to end of DFSpath */
		visited[ iStart * width + jStart ] = 1;
		int lenDFSpath = strlen (DFSpath);
		DFSpath[lenDFSpath] = field[iStart * width + jStart];

		/* If the algorithm inserts the missing letters before, the way in which the search
		 * has come to this cell is a potential result */
		if (isFill)
		{
			/* If the current word are longer than previous result, algorithm will replace last */
			if (databaseFindString(base, DFSpath) && lenDFSpath + 1 > strlen (result))
			{
				result = strcpy(result, DFSpath);
				*location = bufLocation;
				*inserted = bufInserted;
			}

			/* If lengths of this words equal, replacement occurs with a probability of 33% */
			else if (databaseFindString(base, DFSpath) &&
						lenDFSpath + 1 == strlen (result) &&
						rand() % 3 == 0)
			{
				result = strcpy(result, DFSpath);
				*location = bufLocation;
				*inserted = bufInserted;
			}
		}
	/* This code part is a macro that checks the next path and calls the next search iteration */
	#define GO_NEXT \
        if (iNext > -1 && iNext < height && jNext > -1 && jNext < width && /* check next cell location */ \
			visited[iNext * width + jNext] == 0) /* search can't go to visited cell */ \
				DFSstart(field, visited, iNext, jNext, height, \
							width, base, DFSpath, result, isFill, \
							location, bufLocation, inserted, bufInserted);  /* go next */

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
		unsigned char symbol;
		for (symbol = START_SYMBOL; symbol <= STOP_SYMBOL; symbol++)
		{
			/* insert this letter */
			field[iStart * width + jStart] = symbol;

			/* and do previous operations */
			visited[ iStart * width + jStart ] = 1;
			int lenDFSpath = strlen (DFSpath);
			DFSpath[lenDFSpath] = field[iStart * width + jStart];

			if (databaseFindString(base, DFSpath) && lenDFSpath + 1 > strlen (result))
			{
				result = strcpy(result, DFSpath);
				*location = iStart * width + jStart;
				*inserted = symbol;
			}
			else if (databaseFindString(base, DFSpath) &&
						lenDFSpath + 1 == strlen (result) &&
						rand() % 3 == 0)
			{
				result = strcpy(result, DFSpath);
				*location = iStart * width + jStart;
				*inserted = symbol;
			}
			#define GO_NEXT \
			if (iNext > -1 && iNext < height && jNext > -1 && jNext < width && \
				visited[iNext * width + jNext] == 0) \
					DFSstart(field, visited, iNext, jNext, height, \
								width, base, DFSpath, result, 1,\
								location, iStart * width + jStart, inserted, symbol);

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

