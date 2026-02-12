#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

wchar_t path1[] = L"C:\\Users\\user\\source\\repos\\OperationSystems\\shared_memory\\memory_consumer_1\\x64\\Debug\\memory_consumer_1.exe";
wchar_t path2[] = L"C:\\Users\\user\\source\\repos\\OperationSystems\\shared_memory\\memory_consumer_2\\x64\\Debug\\memory_consumer_2.exe";


int main()
{
  srand(static_cast<unsigned int>(time(0)));

  // создание Ивентов
  HANDLE hDeleteNegativeElementsEvent, hSortElementsEvent, hReadyToOutPut;

  hDeleteNegativeElementsEvent = CreateEvent(NULL, TRUE, FALSE, L"Delete");
  hSortElementsEvent = CreateEvent(NULL ,TRUE, FALSE, L"Sort");
  hReadyToOutPut = CreateEvent(NULL, TRUE, FALSE, L"Output");

  // первый процесс
  STARTUPINFO siFirstProducer;
  PROCESS_INFORMATION piFirstProducer;

  ZeroMemory(&siFirstProducer, sizeof(siFirstProducer));
  siFirstProducer.cb = sizeof(siFirstProducer);
  ZeroMemory(&piFirstProducer, sizeof(piFirstProducer));

  // второй процесс
  STARTUPINFO siSecondProducer;
  PROCESS_INFORMATION piSecondProducer;

  ZeroMemory(&siSecondProducer, sizeof(siSecondProducer));
  siSecondProducer.cb = sizeof(siSecondProducer);
  ZeroMemory(&piSecondProducer, sizeof(piSecondProducer));


  int digitsQuontity;
  cout << "Enter the size of your array: ";
  while (!(cin >> digitsQuontity)) {
    cout << "Invalid input" << endl;
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
  }

  int sizeInBytes = (digitsQuontity * sizeof(int) + sizeof(int));
  HANDLE hSharedArray = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeInBytes, L"sharedArray");

  void* pointerBuff = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
  int* sharedArray = (int*)pointerBuff;

  sharedArray[0] = digitsQuontity;
  for (int i = 1; i < digitsQuontity + 1; i++) {
    sharedArray[i] = (rand() % 2001) - 1000;
  }

  // Теперь вызываем CreateProcess для первого процесса
  if (!CreateProcess(NULL, path1, NULL, NULL, FALSE, 0, NULL, NULL, &siFirstProducer, &piFirstProducer)) {
    cout << "Error starting Consumer 1: " << GetLastError() << endl;
  }

  if (!CreateProcess(NULL, path2, NULL, NULL, FALSE, 0, NULL, NULL, &siSecondProducer, &piSecondProducer)) {
    cout << "Error starting Consumer 1: " << GetLastError() << endl;
  }

  SetEvent(hDeleteNegativeElementsEvent);
  WaitForSingleObject(hReadyToOutPut, INFINITE);

  cout << "Sorted array: " << endl;
  for (int i = 1; i < digitsQuontity + 1; i++) {
    cout << sharedArray[i] << ", ";
  }

  CloseHandle(hDeleteNegativeElementsEvent);
  CloseHandle(hSortElementsEvent);
  CloseHandle(hReadyToOutPut);

  UnmapViewOfFile(pointerBuff);
  CloseHandle(hSharedArray);
  return 0;
}
