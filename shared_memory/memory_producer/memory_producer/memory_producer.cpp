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

  HANDLE hDataFilled1  = CreateEvent(NULL, FALSE, FALSE, L"DataFilled1");
  HANDLE hDataFilled2 = CreateEvent(NULL, FALSE, FALSE, L"DataFilled2");
  HANDLE hWriteAccess = CreateEvent(NULL, FALSE, FALSE, L"WriteAccess");
  HANDLE hConsumer1   = CreateEvent(NULL, FALSE, FALSE, L"Consumer1");
  HANDLE hConsumer2   = CreateEvent(NULL, FALSE, FALSE, L"Consumer2");

  HANDLE hFile = CreateFile(
    fileName,
    GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
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
  
  int sizeInBytes = (digitsQuontity * sizeof(int) + sizeof(int));
  HANDLE hSharedArray = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeInBytes, L"sharedArray");

  void* pointerBuff = MapViewOfFile(hSharedArray, FILE_MAP_ALL_ACCESS, 0, 0, 0);
  int* sharedArray = (int*)pointerBuff;

  sharedArray[0] = digitsQuontity;

  for (int i = 1; i < digitsQuontity + 1; i++) {
    sharedArray[i] = (rand() % 2001) - 1000;
  }


  if (!CreateProcess(NULL, path1, NULL, NULL, FALSE, 0, NULL, NULL, &siFirstProducer, &piFirstProducer)) {
    cout << "Error starting Consumer 1: " << GetLastError() << endl;
    return 1;
  }
  cout << "Consumer 1 started (PID: " << piFirstProducer.dwProcessId << ")" << endl;

  if (!CreateProcess(NULL, path2, NULL, NULL, FALSE, 0, NULL, NULL, &siSecondProducer, &piSecondProducer)) {
    cout << "Error starting Consumer 2: " << GetLastError() << endl;
    return 1;
  }

  Sleep(100);

  SetEvent(hDataFilled1);
  SetEvent(hDataFilled2);

  
  WaitForSingleObject(hConsumer1, INFINITE);

  WaitForSingleObject(hConsumer2, INFINITE);
  
  HANDLE processes[] = { piFirstProducer.hProcess, piSecondProducer.hProcess };
  WaitForMultipleObjects(2, processes, TRUE, INFINITE);


  cout << "Check results.txt for output" << endl;
  

  CloseHandle(piFirstProducer.hProcess);
  CloseHandle(piFirstProducer.hThread);
  CloseHandle(piSecondProducer.hProcess);
  CloseHandle(piSecondProducer.hThread);
  CloseHandle(hDataFilled1);
  CloseHandle(hDataFilled2);
  CloseHandle(hWriteAccess);
  CloseHandle(hConsumer1);
  CloseHandle(hConsumer2);
  UnmapViewOfFile(pointerBuff);
  CloseHandle(hSharedArray);

  return 0;
}
