// merge_sort.cpp : Определяет экспортируемые функции для DLL.
#include "pch.h"
#include "framework.h"
#include "merge_sort.h"
#include <vector>

using namespace std;

 vector<int> merge(const  vector<int>& leftVector, const  vector<int>& rightVector) {
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

 vector<int> merge_sort( vector<int>& array) {
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

   leftVector = merge_sort(leftVector);
   rightVector = merge_sort(rightVector);

   return merge(leftVector, rightVector);
}

// ЭКСПОРТИРУЕМАЯ ФУНКЦИЯ
MERGESORT_API void runMergeSort( vector<int>& array) {
  if (array.empty()) return;
  array = merge_sort(array);
}

