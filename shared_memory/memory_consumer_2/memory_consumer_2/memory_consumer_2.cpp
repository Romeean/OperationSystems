#include <iostream>
#include <windows.h>
#include <vector>
#include <set>

using namespace std;


int main()
{
	
	HANDLE hDataFilled = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"DataFilled");
	HANDLE hSharedArray = OpenFileMapping(FILE_ALL_ACCESS, FALSE, L"sharedArray");

	// ЭТУ ЧАСТЬ НУЖНО ПОДРОБНЕЕ ИЗУЧИТЬ
	void* pointerBuffer = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int* sharedArray = (int*)pointerBuffer;

	int arrayLength = sharedArray[0];
	int middle = arrayLength / 2;
	
	// СОРТИРВОКА ВТОРОЙ ПОЛОВИНЫ МАССИВА
	WaitForSingleObject(hDataFilled, INFINITE);
	
	vector<int> secondTaskVec;

	for (int i = 1; i < arrayLength; i++) {
		secondTaskVec.push_back(sharedArray[i]);
	}

	set<int> unDuplicatedSet;

	for (int i = 0; i < secondTaskVec.size(); i++) {
		unDuplicatedSet.insert(secondTaskVec[i]);
	}

	UnmapViewOfFile(pointerBuffer);
	CloseHandle(hSharedArray);
	CloseHandle(hDataFilled);

	return 0;
}