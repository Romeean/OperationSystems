#include <vector>

using namespace std;

// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа MERGESORT_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции MERGESORT_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef MERGESORT_EXPORTS
#define MERGESORT_API __declspec(dllexport)
#else
#define MERGESORT_API __declspec(dllimport)
#endif;

extern "C" {
	MERGESORT_API void MergeSort( vector<int>& array );
}
