#include <iostream>
#include <windows.h>
#include <vector>


using namespace std;


wchar_t path[] = L"C:\\Users\\user\\source\\repos\\OperationSystems\\pipe\\pipe_client\\x64\\Debug\\pipe_client.exe";

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

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, dwSize)) {
		cout << "Error with pipe: " << GetLastError() << endl;
	}
	if (!CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		cout << "Error with process: " << GetLastError() << endl;
	}

	si.hStdInput	= hReadPipe;
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError	= GetStdHandle(STD_ERROR_HANDLE);


		


	
	


	int choice = -1;
	while (true) {
		
		cout << "What you want to send to client?" << endl;
		cout << "1. create an array and sent it to the client and sort it." << endl;
		cout << "2. create an array and sent it to the client and cout all of the unique elements.  " << endl;
		
		while (!(cin >> choice)) {
			cout << "Invalid input. Try again";
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}


		switch (choice) {
		
		case 1: {
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
			// передача с помощьб pipe значения длинны массива который ввел пользователь
			WriteFile(hWritePipe, &length, sizeof(int), &dwWritten, NULL);
			
			WriteFile(hWritePipe, array.data(), length & sizeof(int), &dwWritten, NULL);



		}

		case 2: {

		}
		default: {

		}
		}
		cout << endl;
	}





	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}