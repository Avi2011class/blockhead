#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#include "computerNext_utf-8.h"

#define SIZE 3
int main (void)
{
	setlocale (LC_ALL, "ru_RU.utf8");
	srand(time(0));

	struct StringDatabase* D = databaseInit();
	FILE* databaseFile = fopen ("zdf-win.txt", "r");
	databaseRead(D, databaseFile);
	fclose (databaseFile);
	wchar_t array[SIZE * SIZE + 2] = {};
	wchar_t result[SIZE * SIZE + 2] = {};
	wchar_t* startWord = L"дом";
	uint8_t startPositionX = 1;
	uint8_t startPositionY = 1;
	memcpy(array + ( (startPositionY - 1) * SIZE + startPositionX - 1),
			startWord, sizeof (wchar_t) * wcslen (startWord));
	databaseRemove(D, startWord);

	#define PRINT_FIELD(field) \
	{\
		int i, j;\
		for (i = 0; i < SIZE; i++)\
			{ \
				for ( j = 0; j < SIZE; j++) \
					wprintf(L"%lc  ", array[SIZE * i + j] ? array[SIZE * i + j] : L'-');\
				wprintf (L"\n");\
			}\
	}
	PRINT_FIELD(array);

	int k;
	for (k = 0; k < SIZE * SIZE; k++)
	{
		int location = 0;
		wchar_t inserted = 0;
		if( computerNext (array, SIZE, SIZE, D, result, &location, &inserted) == 0)
            break;
		wprintf (L"result = %ls, location = (%d, %d), inserted = %lc\n", \
				result, location / SIZE + 1, location % SIZE + 1, inserted);

	    databaseRemove(D, result);
	    memset(result, 0, sizeof(wchar_t) * SIZE * SIZE);
	    array[location] = inserted;
	    PRINT_FIELD(array);
	    if (isFieldFull(array, SIZE, SIZE))
            break;
	    wprintf(L"isFull = %hhu\n", isFieldFull(array, SIZE, SIZE));
	}

	databaseDestroy(D);
	#undef PRINT_FIELD

    return 0;
}
