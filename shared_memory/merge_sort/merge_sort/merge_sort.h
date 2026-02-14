#pragma once // Важно, чтобы файл не подключался дважды

#ifdef MERGESORT_EXPORTS
#define MERGESORT_API __declspec(dllexport)
#else
#define MERGESORT_API __declspec(dllimport)
#endif

#include <vector>

// Оборачиваем в extern "C", чтобы C++ не "искажал" имя функции.
// Это гарантирует, что линкер найдет функцию runMergeSort именно под этим именем.
extern "C" {
  // Твоя функция экспорта
  MERGESORT_API void runMergeSort(std::vector<int>& array);
}
