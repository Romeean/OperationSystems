#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;

wchar_t path1[] = L"C:\\Users\\user\\source\\repos\\OperationSystems\\shared_memory\\memory_consumer_1\\x64\\Debug\\memory_consumer_1.exe";
wchar_t path2[] = L"C:\\Users\\user\\source\\repos\\OperationSystems\\shared_memory\\memory_consumer_2\\x64\\Debug\\memory_consumer_2.exe";
wchar_t fileName[] = L"results.txt";

int main()
{
  srand(static_cast<unsigned int>(time(0)));

  HANDLE hDataFilled;
  hDataFilled = CreateEvent(NULL, TRUE, FALSE, L"DataFilled");

  HANDLE hFile = CreateFile(
    fileName,
    GENERIC_WRITE,
    0,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL
    );
  if (!hFile) {
    cout << "Could not open file, error: " << GetLastError() << endl;
  }


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
  
  if (!CreateProcess(NULL, path1, NULL, NULL, FALSE, 0, NULL, NULL, &siFirstProducer, &piFirstProducer)) {
    cout << "Error starting Consumer 1: " << GetLastError() << endl;
  }

  if (!CreateProcess(NULL, path2, NULL, NULL, FALSE, 0, NULL, NULL, &siSecondProducer, &piSecondProducer)) {
    cout << "Error starting Consumer 1: " << GetLastError() << endl;
  }

  int sizeInBytes = (digitsQuontity * sizeof(int) + sizeof(int));
  HANDLE hSharedArray = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeInBytes, L"sharedArray");

  void* pointerBuff = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
  int* sharedArray = (int*)pointerBuff;

  sharedArray[0] = digitsQuontity;
  for (int i = 1; i < digitsQuontity + 1; i++) {
    sharedArray[i] = (rand() % 2001) - 1000;
  }
  SetEvent(hDataFilled);


  // Запись в файл
  ofstream outFile("results.txt", ios::app);
  if (outFile.is_open()) {
    outFile << "Consumer 2 Results:\n";
    outFile << "Original size: " << secondTaskVec.size() << "\n";
    outFile << "Unique elements: " << unDuplicatedSet.size() << "\n";
    outFile << "Unique values: ";
    for (int val : unDuplicatedSet) {
      outFile << val << " ";
    }
    outFile << "\n";
    outFile.close();
  }
 





  CloseHandle(hDataFilled);
  UnmapViewOfFile(pointerBuff);
  CloseHandle(hSharedArray);
  CloseHandle(piFirstProducer.hThread);
  CloseHandle(piFirstProducer.hProcess);
  CloseHandle(piSecondProducer.hThread);
  CloseHandle(piSecondProducer.hProcess);


  return 0;
}
