#ifndef COMPUTERNEXT_H_INCLUDED
#define COMPUTERNEXT_H_INCLUDED

#include <stdint.h>
#include "insertion-control_utf-8.h"
#include "database_utf-8.h"

#define DEEP_ALGORITHM

int computerNext (wchar_t* field, int height, int width,
				struct StringDatabase* base, wchar_t* result, int *location, wchar_t* inserted);
/*
 * This method returns preferable computer action in this game field
 * field - pointer to field in linear format
 * height, width - field's height and width
 * base - pointer to words base
 * result - result recording address
 * location - inserted letter location address
 * inserted - inserted letter address

 * this method returns wcslen (result) or NULL if search fails
*/

uint8_t isFieldFull (wchar_t* field, int height, int width);

#endif // COMPUTERNEXT_H_INCLUDED
