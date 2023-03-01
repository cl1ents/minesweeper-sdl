typedef struct Array {
	int* array;
	int length;
	int size;
} Array;

Array newArray();
Array newArraySize(int size);
void initArraySize(Array* arr, int size);
void initArray(Array* arr);
void insertInto(Array* arr, int toAppend);
void insertIntoIndex(Array* arr, int toAppend, int index);
int removeFrom(Array* arr, int index);