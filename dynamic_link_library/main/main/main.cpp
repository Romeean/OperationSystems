#include <iostream>
#include <windows.h>
#include <vector>
#include <list.h>

using namespace std;


typedef void (*mergesort)(vector<int>&);

int main()
{
  vector<int> array = { 124,1254,346,5,4356,243,2,63,456,234,2,634,56,34 };
  int choice = -1;

  while (true) {
    cout << endl;
    cout << "1. Sort vector with .dll obviously. " << endl;
    cout << "2. Do some action with .dll that import's implicitly a List class. " << endl;
    cout << "Your choice: ";
    while (!(cin >> choice)) {
      cout << "Invalid input. Try again." << endl;
      cin.clear();
      cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }

    switch (choice) {
      case 1: {
        HMODULE hDLL = LoadLibrary(L"C:\\Users\\user\\source\\repos\\OperationSystems\\dynamic_link_library\\merge_sort\\x64\\Debug\\mergesort.dll");
        if (!hDLL) {
          cout << "Couldn't find a DLL " << endl;
        }
        else {
          cout << "Did found a specific library" << endl;
        }
        mergesort MyMergeSort = (mergesort)GetProcAddress(hDLL, "MergeSort");

        MyMergeSort(array);
        for (int number : array) {
          cout << number << " " << endl;
        }
        FreeLibrary(hDLL);
        break;
      };
      case 2: {
        List myList;
        myList.add(10);
        myList.add(20);
        myList.add(30);
        myList.add(40);
        
        cout << "Lists size: " << myList.size() << endl;

        myList.print();
      
        break;
      }
      default :{
        cout << "Invalid input. Try again." << endl; 
      }
    }
  }
  return 0;
}
