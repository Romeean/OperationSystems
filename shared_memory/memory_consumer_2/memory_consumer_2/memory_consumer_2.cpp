#include <iostream>
#include <windows.h>
#include <vector>
#include <set>
#include <merge_sort.h>

using namespace std;


int main()
{
	HANDLE hSharedArray = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"sharedArray");
	HANDLE hDeleteNegativeElementsEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Delete");
	HANDLE hSortElementsEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Sort");
	HANDLE hReadyToOutPut = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Output");

	// ЭТУ ЧАСТЬ НУЖНО ПОДРОБНЕЕ ИЗУЧИТЬ
	void* pointerBuffer = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int* sharedArray = (int*)pointerBuffer;

	int arrayLength = sharedArray[0];
	int firstPart = arrayLength / 2;
	
	// СОРТИРВОКА ВТОРОЙ ПОЛОВИНЫ МАССИВА
	WaitForSingleObject(hSortElementsEvent, INFINITE);
	ResetEvent(hSortElementsEvent);
	
	vector<int> secondTaskVec;

	for (int i = i; i < arrayLength; i++) {
		secondTaskVec.push_back(sharedArray[i]);
	}

	set<int> unDuplicatedSet;

	for (int i = 0; i < secondTaskVec.size(); i++) {
		unDuplicatedSet.insert(secondTaskVec[i]);
	}

	SetEvent(hReadyToOutPut);
	CloseHandle(hDeleteNegativeElementsEvent);
	CloseHandle(hSortElementsEvent);
	CloseHandle(hReadyToOutPut);
	return 0;
}