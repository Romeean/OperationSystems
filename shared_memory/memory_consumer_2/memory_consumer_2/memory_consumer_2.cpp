#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;


vector<int> merge(const vector<int>& leftVector, const vector<int>& rightVector) {
	int leftIndex = 0;
	int rightIndex = 0;

	vector<int> result;

	while (leftIndex < leftVector.size() && rightIndex < rightVector.size()) {
		
		if (leftVector[leftIndex] > rightVector[rightIndex]) {
			result.push_back(rightVector[rightIndex]);
			rightIndex++;
		}
		else { 
			result.push_back(leftVector[leftIndex]);
			leftIndex++;
		}

	}

	while (leftIndex < leftVector.size()) {
		result.push_back(leftVector[leftIndex]);
		leftIndex++;
	}
	while (rightIndex < rightVector.size()) {
		result.push_back(rightVector[rightIndex]);
		rightIndex++;
	}

	return result;
}

vector<int> mergeSort(vector<int>& array) {
	if (array.size() == 1) return array;
	int arraySize = array.size();

	int left = 0;
	int middle = arraySize / 2;
	int right = arraySize;

	vector<int> leftVector, rightVector;

	for (int i = 0; i < middle; i++) {
		leftVector.push_back(array[i]);
	}

	for (int i = middle; i < right; i++) {
		rightVector.push_back(array[i]);
	}

	leftVector = mergeSort(leftVector);
	rightVector = mergeSort(rightVector);

	return merge(leftVector, rightVector);
}


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
	vector<int> arrayFirstPart;
	
	// СОРТИРВОКА ПЕРВОЙ ПОЛОВИНЫ МАССИВА
	WaitForSingleObject(hSortElementsEvent, INFINITE);
	ResetEvent(hSortElementsEvent);
	for (int i = 1; i < firstPart; i++) {
		arrayFirstPart.push_back(sharedArray[i]);
	}
	arrayFirstPart = mergeSort(arrayFirstPart);
	for (int i = 0; i < arrayFirstPart.size(); i++) {
		sharedArray[i + 1] = arrayFirstPart[i];
	}
	SetEvent(hDeleteNegativeElementsEvent);

	// СОРТИРВОКА ВТОРОЙ ПОЛОВИНЫ МАССИВА
	WaitForSingleObject(hSortElementsEvent, INFINITE);
	ResetEvent(hSortElementsEvent);
	vector<int> arraySecondPart;
	for (int i = firstPart; i <= arrayLength; i++) {
		arraySecondPart.push_back(sharedArray[i]);
	}
	arraySecondPart = mergeSort(arraySecondPart);
	int k = 0;
	for (int i = firstPart; i <= arrayLength; i++) {
		sharedArray[i] = arraySecondPart[k];
		k++;
	}

	vector<int> result;

	for (int i = 1; i < firstPart; i++) {
		result.push_back(sharedArray[i]);
	}
	for (int i = firstPart; i < arrayLength; i++) {
		result.push_back(sharedArray[i]);
	}

	result = mergeSort(result);

	for (int i = 1; i < arrayLength; i++) {
		sharedArray[i] = result[i - 1];
	}


	SetEvent(hReadyToOutPut);
	CloseHandle(hDeleteNegativeElementsEvent);
	CloseHandle(hSortElementsEvent);
	CloseHandle(hReadyToOutPut);
	return 0;
}