#include <iostream>
#include <windows.h>
#include <vector>
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
	
	// СОРТИРВОКА ПЕРВОЙ ПОЛОВИНЫ МАССИВА
	WaitForSingleObject(hSortElementsEvent, INFINITE);
	ResetEvent(hSortElementsEvent);
	vector<int> secondPartVec;
	for (int i = firstPart; i <= arrayLength; i++) {
		secondPartVec.push_back(sharedArray[i]);
	}

	runMergeSort(secondPartVec);

	for (int i = 0; i < secondPartVec.size(); i++) {
		sharedArray[firstPart + i] = secondPartVec[i];
	}
	SetEvent(hDeleteNegativeElementsEvent);

	WaitForSingleObject(hSortElementsEvent, INFINITE);
	ResetEvent(hSortElementsEvent);
	
	vector<int> arrayFirstPart;
	for (int i = 1; i <= firstPart; i++) {
		if (sharedArray[i] > -1) {
			arrayFirstPart.push_back(sharedArray[i]);
		}
		else {
			arrayFirstPart.push_back(0);
		}
	}
	
	for (int i = 1; i <= arrayLength; i++) {
		sharedArray[i] = arrayFirstPart[i + 1];
	}

	
	SetEvent(hReadyToOutPut);
	CloseHandle(hDeleteNegativeElementsEvent);
	CloseHandle(hSortElementsEvent);
	CloseHandle(hReadyToOutPut);
	return 0;
}