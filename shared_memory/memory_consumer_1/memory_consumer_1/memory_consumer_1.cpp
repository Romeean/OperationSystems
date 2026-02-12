#include <iostream>
#include <windows.h>
#include "merge_sort.h"

using namespace std;


int main()
{
	HANDLE hSharedArray = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"sharedArray");
	void* pointerBuffer = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int* sharedArray = (int*)pointerBuffer;

	HANDLE hDeleteNegativeElementsEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Delete");
	HANDLE hSortElementsEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Sort");

	int arrayLength = sharedArray[0];
	int firstPart = arrayLength / 2;


	WaitForSingleObject(hDeleteNegativeElementsEvent, INFINITE);
	ResetEvent(hDeleteNegativeElementsEvent);
	
	vector<int> sortedFirstPart;
	for (int i = 1; i < firstPart; i++) {
		sortedFirstPart.push_back(sharedArray[i]);
	}
	runMergeSort(sortedFirstPart);

	for (int i = 1; i < firstPart; i++) {
		sharedArray[i] = sortedFirstPart[i - 1];
	}

	SetEvent(hSortElementsEvent);
	
	WaitForSingleObject(hDeleteNegativeElementsEvent, INFINITE);
	ResetEvent(hDeleteNegativeElementsEvent);

	vector<int> secondPartOfArray;
	for (int i = firstPart; i < arrayLength; i++) {
		if (sharedArray[i] >= -1) {
			secondPartOfArray.push_back(sharedArray[i]);
		}
		else {
			secondPartOfArray.push_back(0);
		}
	}

	int j = 0;
	for (int i = firstPart; i < arrayLength; i++) {
		sharedArray[i] = secondPartOfArray[j];
		j++;
	};

	SetEvent(hSortElementsEvent);
	ResetEvent(hDeleteNegativeElementsEvent);

	
	CloseHandle(hDeleteNegativeElementsEvent);
	CloseHandle(hSortElementsEvent);
	CloseHandle(hSharedArray);
	return 0;
}
