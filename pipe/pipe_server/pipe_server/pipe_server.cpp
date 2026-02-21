#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;


//wchar_t path_pc[] = L"C:\\Users\\user\\source\\repos\\OperationSystems\\pipe\\pipe_client\\x64\\Debug\\pipe_client.exe";
wchar_t path_desctop[] = L"C:\\Users\\Владимир\\source\\repos\\OperationSystems\\pipe\\pipe_client\\x64\\Debug\\pipe_client.exe";


int main()
{
	HANDLE hWritePipe, hReadPipe;
	DWORD dwSize = 0;

	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, dwSize)) {
		cout << "Error with pipe: " << GetLastError() << endl;
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESTDHANDLES;

	si.hStdInput = hReadPipe;
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);


	if (!CreateProcess(NULL, path_desctop, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		cout << "Error with process: " << GetLastError() << endl;
	}
		
	cout << "Create an array and sent it to the client." << endl;
	
	vector<int> array;
	size_t length;

	cout << "Enter desired length of the array: ";
	while (!(cin >> length)) {
		cout << "Invalid input. Try again" << endl;
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}
	for (size_t i = 0; i < length; i++) {
		int number;
		cout << "Enter " << i + 1 << "value is: ";
		while (!(cin >> number)) {
			cout << "Invalid input. Try again" << endl;
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}
		array.push_back(number);
	}
	
	DWORD dwWritten;
	WriteFile(hWritePipe, &length, sizeof(size_t), &dwWritten, NULL); 
	WriteFile(hWritePipe, array.data(), length * sizeof(int), &dwWritten, NULL); 

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}