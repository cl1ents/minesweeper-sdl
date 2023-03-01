#include <stdlib.h>
#include "Array.h"

Array newArray()
{
	Array ret;
	initArray(&ret);
	return ret;
};

Array newArraySize(int size)
{
	Array ret;
	initArraySize(&ret, size);
	return ret;
};

void initArraySize(Array* arr, int size)
{
	arr->length = 0;
	arr->size = size;
	arr->array = malloc((arr->size) * sizeof (int));
};

void initArray(Array* arr)
{
	initArraySize(arr, 1);
};

void insertIntoIndex(Array* arr, int toAppend, int index)
{
	if (arr->size == arr->length) {
		arr->size *= 2;
		arr->array = realloc(arr->array, (arr->size) * sizeof(int));
	}

	if (arr->length != index)
		memcpy(&arr->array[index + 1], &arr->array[index], (arr->size - index) * sizeof(int));

	arr->length++;
	arr->array[index] = toAppend;
};

void insertInto(Array* arr, int toAppend)
{
	insertIntoIndex(arr, toAppend, arr->length);
};

int removeFrom(Array* arr, int index)
{
	int ret = arr->array[index];
	if (index < arr->length) 
	{
		arr->length--;
		memcpy(&arr->array[index], &arr->array[index + 1], (arr->size - index) * sizeof(int));

		if (arr->length <= arr->size / 2) {
			arr->size /= 2;
			arr->array = realloc(arr->array, (arr->size) * sizeof(int));
		}
	}

	return ret;
};