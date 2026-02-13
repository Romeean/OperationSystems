#include <iostream>
#include <windows.h>
#include "merge_sort.h"
#include <set>

using namespace std;


int main()
{
	HANDLE hSharedArray = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"sharedArray");
	void* pointerBuffer = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int* sharedArray = (int*)pointerBuffer;

	HANDLE hDeleteNegativeElementsEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Delete");
	HANDLE hSortElementsEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Sort");

	int arrayLength = sharedArray[0];
	int middle = arrayLength / 2;


	WaitForSingleObject(hDeleteNegativeElementsEvent, INFINITE);
	ResetEvent(hDeleteNegativeElementsEvent);

	vector<int> firstTaskVec;
	for (int i = 1; i < arrayLength; i++) {
		firstTaskVec.push_back(sharedArray[i]);
	}
	runMergeSort(firstTaskVec);

	
	int sum = 0;

	for (int i = 0; i < firstTaskVec.size(); i++) {
		sum += firstTaskVec[i];
	}

	int arithmeticMean = sum / firstTaskVec.size();


	int median = 0;
	if (firstTaskVec.size() % 2 == 0) {
		median = ((firstTaskVec.size() / 2 - 1) + (firstTaskVec.size() / 2)) / 2;
	}
	else {
		median = firstTaskVec.size() / 2;
	}

	



	
	CloseHandle(hDeleteNegativeElementsEvent);
	CloseHandle(hSortElementsEvent);
	CloseHandle(hSharedArray);
	return 0;
}
