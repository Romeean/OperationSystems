#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

vector<int> merge(vector<int>& left, vector<int>& right) {
	int leftIndex = 0;
	int rightIndex = 0;

	vector<int> result;

	while (leftIndex < left.size() && rightIndex < right.size()) {
		if (left[leftIndex] > right[rightIndex]) {
			result.push_back(right[rightIndex]);
			rightIndex++;
		}
		else {
			result.push_back(left[leftIndex]);
			leftIndex++;
		}
	}
	while (leftIndex < left.size()) {
		result.push_back(left[leftIndex]);
		leftIndex++;
	}
	while (rightIndex < right.size()) {
		result.push_back(right[rightIndex]);
		rightIndex++;
	}
	return result;
}

vector<int> merge_sort(vector<int> &array) {
	if (array.size() == 1) return array;
	int middle = array.size() / 2;

	int length = array.size();
	
	vector<int> left(array.begin(), array.begin() + middle);
	vector<int> right(array.begin() + middle, array.end());

	left  = merge_sort(left);
	right = merge_sort(right);

	return merge(left, right);
}


int main()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD bytesRead;
	size_t length;

	if (!ReadFile(hInput, &length, sizeof(size_t), &bytesRead, 0)) {
		cout << "Read length error: " << GetLastError() << endl;
		return 1;
	};

	vector<int> loadedArray(length);
	if (!ReadFile(hInput, loadedArray.data(), length * sizeof(int), &bytesRead, 0)) {
		cout << "Read array error: " << GetLastError() << endl;
		return 1;
	}

	cout << "sorted array" << endl;
	
	vector<int> result = merge_sort(loadedArray);
	cout << "[ ";
	for (int i = 0; i < result.size(); i++) {
		if (i == result.size() - 1) {
			cout << result[i] << " ";
			break;
		}
		cout << result[i] << ", ";
	}
	cout << "]";

	return 0;
}	
