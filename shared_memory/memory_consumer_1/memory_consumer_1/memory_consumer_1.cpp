#include <iostream>
#include <windows.h>

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
	for (int i = 1; i < firstPart; i++) {
		if (sharedArray[i] < 0) {
			sharedArray[i] = 0;
		}
	}
	cout << "Consumer 1 delete srabotal " << endl;

	SetEvent(hSortElementsEvent);
	
	WaitForSingleObject(hDeleteNegativeElementsEvent, INFINITE);
	ResetEvent(hDeleteNegativeElementsEvent);

	for (int i = firstPart; i <= arrayLength; i++) {
		if (sharedArray[i] < 0) {
			sharedArray[i] = 0;
		}
	}
	cout << "Consumer 2 delete srabotal " << endl;
	SetEvent(hSortElementsEvent);
	ResetEvent(hDeleteNegativeElementsEvent);

	
	CloseHandle(hDeleteNegativeElementsEvent);
	CloseHandle(hSortElementsEvent);
	return 0;
}
