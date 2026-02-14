#include <set>
#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "merge_sort.h"

using namespace std;


int GetMedianSortedArray(vector<int>& sortedArray) {
	int median = 0;
	if (sortedArray.size() % 2 == 0) {
		median = (sortedArray[sortedArray.size() / 2 - 1] +
			sortedArray[sortedArray.size() / 2]) / 2;
	}
	else {
		median = sortedArray[sortedArray.size() / 2];
	}
	return median;
}

int main()
{
	HANDLE hSharedArray = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"sharedArray");
	HANDLE hDataFilled1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"DataFilled1");
	HANDLE hWriteAccess = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"WriteAccess");
	HANDLE hConsumer1		= OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Consumer1");
	HANDLE hFile				=	CreateFile(L"results.txt", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	WaitForSingleObject(hDataFilled1, INFINITE);

	void* pointerBuffer = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int* sharedArray = (int*)pointerBuffer;
	int arrayLength = sharedArray[0];
	int middle = arrayLength / 2;

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
	int median = GetMedianSortedArray(firstTaskVec);
	SetFilePointer(
		hFile,
		0,
		NULL,
		FILE_END
	);

	ostringstream oss;
	oss << "CONSUMER 1 RESULTS (Second half)\n";
	oss << "Original size: " << firstTaskVec.size() << "\n";
	for (int i = 0; i < firstTaskVec.size(); i++) {
		oss << firstTaskVec[i] << ", ";
	}
	oss << "\n";
	oss << "Arithmetic mean: " << arithmeticMean << "\n";
	oss << "Median: " << median << "\n";

	string data = oss.str();
	DWORD bytesWritten;
	BOOL success = WriteFile(hFile, data.c_str(), data.length(), &bytesWritten, NULL);

	if (success) {
		cout << "Consumer 1: Successfully wrote " << bytesWritten << " bytes" << endl;
	}
	else {
		cout << "Consumer 1: Write failed. Error: " << GetLastError() << endl;
	}


	SetEvent(hWriteAccess);

	UnmapViewOfFile(pointerBuffer);
	CloseHandle(hSharedArray);
	CloseHandle(hDataFilled1);
	
	CloseHandle(hWriteAccess);
	
	SetEvent(hConsumer1);

	CloseHandle(hConsumer1);
	return 0;
}
