#include <iostream>
#include <windows.h>
#include <sstream>
#include <vector>
#include <set>

using namespace std;


int main()
{
	HANDLE hDataFilled2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"DataFilled2");
	HANDLE hSharedArray = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"sharedArray");
	HANDLE hWriteAccess = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"WriteAccess");
	HANDLE hConsumer2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Consumer2");
	HANDLE hFile = CreateFile(
		L"results.txt",
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	WaitForSingleObject(hDataFilled2, INFINITE);
	void* pointerBuffer = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int* sharedArray = (int*)pointerBuffer;
	int arrayLength = sharedArray[0];
	int middle = arrayLength / 2;
	vector<int> secondTaskVec;
	
	for (int i = 1; i < arrayLength; i++) {
		if (!(sharedArray[i] < -1)){
			secondTaskVec.push_back(sharedArray[i]);
		}
	}
	set<int> unDuplicatedSet;
	for (int i = 0; i < secondTaskVec.size(); i++) {
		unDuplicatedSet.insert(secondTaskVec[i]);
	}

	WaitForSingleObject(hWriteAccess, INFINITE);
	SetFilePointer(
		hFile,      
		0,          
		NULL,       
		FILE_END    
	);

	ostringstream oss;
	oss << "CONSUMER 2 RESULTS (Second half)\n";
	oss << "Original size: " << arrayLength << "\n";
	oss << "Unique elements: " << unDuplicatedSet.size() << "\n";
	oss << "Duplicates removed: " << (secondTaskVec.size() - unDuplicatedSet.size()) << "\n";
	oss << "All unique values: ";
	int count = 0;
	for (int val : unDuplicatedSet) {
		oss << val << ", ";
	}
	
	string data = oss.str();
	DWORD bytesWritten;
	BOOL success = WriteFile(hFile, data.c_str(), data.length(), &bytesWritten, NULL);
	
	

	SetEvent(hConsumer2);

	UnmapViewOfFile(pointerBuffer);
	
	CloseHandle(hSharedArray);
	CloseHandle(hDataFilled2);
	CloseHandle(hWriteAccess);
	CloseHandle(hConsumer2);
	return 0;
}