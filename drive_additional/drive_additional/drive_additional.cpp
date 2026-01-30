#include <iostream>
#include <windows.h>

using namespace std;

// Перший потік повинен змінювати атрибути файлів в заданій директорії.
// Другий потік повинен повідомляти користувача про зміни у файловій системі. ReadDirectoryChangesW


HANDLE hChangeAttributesThread, hOutputAttributesChangeThread;
DWORD dwChangeAttributesId, hOutputAttributesChangeId;

string OutputAttributeEncode(DWORD element) {
	
	if (element == INVALID_FILE_ATTRIBUTES) return "Ooh, invalid attributes";


	if (element & FILE_ATTRIBUTE_READONLY) return " Readonly attribute";
	if (element & FILE_ATTRIBUTE_HIDDEN) return " Hidden attribute";
	if (element & FILE_ATTRIBUTE_SYSTEM) return " System attribute";
	if (element & FILE_ATTRIBUTE_DIRECTORY) return " Directory attribute";
	if (element & FILE_ATTRIBUTE_ARCHIVE) return " Archive attribute";
	if (element & FILE_ATTRIBUTE_NORMAL) return " Normal attribute";
	

	return "No attributes at all! Try again.";
}

DWORD WINAPI ChangeFileAttributes(LPVOID lpParam) {
	
	string directory;
	cout << "Write down directory which attribute you want to change: ";
	while (!(cin >> directory)) {
		cout << "Incorrect input! Try again: ";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}
	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	string result = OutputAttributeEncode(currentAttribute);
	cout << "Attributes information of " << directory << ": " << result;


	return 0;
}

DWORD WINAPI OutputAttributesChange(LPVOID lpParams) {
	return 0;
}



int main()
{
	hChangeAttributesThread = CreateThread(NULL, 0, ChangeFileAttributes, NULL, 0, &dwChangeAttributesId);
	hOutputAttributesChangeThread = CreateThread(NULL, 0, OutputAttributesChange, NULL, 0, &dwChangeAttributesId);


	WaitForSingleObject(hChangeAttributesThread, INFINITE);
	WaitForSingleObject(hOutputAttributesChangeThread, INFINITE);

	CloseHandle(hChangeAttributesThread);
	CloseHandle(hOutputAttributesChangeThread);

	return 0;
}
