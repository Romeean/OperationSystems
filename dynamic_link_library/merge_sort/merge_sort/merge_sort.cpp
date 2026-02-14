#include "pch.h"
#include "framework.h"
#include "merge_sort.h"
#include <vector>

using namespace std;


vector<int> merge(vector<int>& left, vector<int>& right) {

  int leftIndex = 0;
  int rightIndex = 0;

  vector<int> result;

  while (leftIndex < left.size() && rightIndex < right.size()) {

    if (left[leftIndex] > right[rightIndex]) {
      result.push_back(left[leftIndex]);
      leftIndex++;
    }
    else {
      result.push_back(right[rightIndex]);
      rightIndex++;
    }
  }

  while(leftIndex < left.size()){
    result.push_back(left[leftIndex]);
    leftIndex++;
  }
  while (rightIndex < right.size()) {
    result.push_back(right[rightIndex]);
    rightIndex++;
  }

  return result;
}
MERGESORT_API vector<int> merge_sort(vector<int> array){
  if (array.size() == 1) {
    return array;
  }

  int length = array.size();
  int middle = length / 2;

  vector<int> left, right;

  for (int i = 0; i < middle; i++) {
    left.push_back(array[i]);
  }

  for (int i = middle; i < length; i++) {
    right.push_back(array[i]);
  }

  left = merge_sort(left);
  right = merge_sort(right);

  return merge(left, right);
}


extern "C" {
  MERGESORT_API void MergeSort(vector<int>& array) {
    if (array.empty()) {
      return;
    }
    array = merge_sort(array);
  }
}


