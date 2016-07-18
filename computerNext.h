#ifndef COMPUTERNEXT_H_INCLUDED
#define COMPUTERNEXT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "database.h"

int computerNext (char* field, int height, int width,
				struct StringDatabase* base, char* result, int *location, char* inserted);
/*
 * This method returns preferable computer action in this game field
 * field - pointer to field in linear format
 * height, width - field's heght and width
 * base - pointr to words base
 * result - result recording address
 * location - inserted letter location address
 * inserted - inserted letter address

 * this method returns strlen(result) or NULL if search fails
*/

void DFSstart (char* field, char* visited,
			int iStart, int jStart, int height, int width,
			struct StringDatabase* base, char* 	DFSpath, char* result, uint8_t isFill,
			int* location, int bufLocation, char* inserted, char bufInserted);

/*
 * This is search method. It returns the longest word that can be put on the field,
 * and that starts at a given position. It's used to bypass the search depth

 * field - pointer to field in linear format
 * iStart, jStart - start position
 * height, width - field's heght and width
 * base - pointr to words base
 * result - result recording address
 * location - inserted letter location address
 * inserted - inserted letter address
*/
#endif // COMPUTERNEXT_H_INCLUDED
