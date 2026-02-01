#include <iostream>
#include <windows.h>

using namespace std;

// Перший потік повинен змінювати атрибути файлів в заданій директорії.
// Другий потік повинен повідомляти користувача про зміни у файловій системі. ReadDirectoryChangesW


DWORD dwToggleObjectAttributeId, dwAddObjectAttributeId, dwDeleteObjectAttributeId;
HANDLE hToggleObjectAttributeThread, hAddObjectAttributeThread, hDeleteObjectAttributeThread;

DWORD dwChangeAttributeObserverId;
HANDLE hChangeAttributeObserverThread;

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
DWORD	AttributeDwordCreator(const string& element) {
	DWORD result = INVALID_FILE_ATTRIBUTES;

	
	if (element == "directory") {
		result = FILE_ATTRIBUTE_DIRECTORY;
		return result;
	};
	if (element == "hidden") {
		result = FILE_ATTRIBUTE_HIDDEN;
		return result;
	}
	if (element == "archive") {
		result = FILE_ATTRIBUTE_ARCHIVE;
		return result;
	}
	if (element == "normal") {
		result = FILE_ATTRIBUTE_NORMAL;
		return result;
	}

	return result;
}


DWORD WINAPI ToggleFileAttribute(const string& directory ) {

	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	DWORD toggledAttribute = currentAttribute ^ currentAttribute;

	bool result = SetFileAttributesA(directory.c_str(), toggledAttribute);

	return 0;
}
DWORD WINAPI AddFileAttribute(const string& directory) {
	string nAttribute;

	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	
	cout << "Which attribute you want to add? Write it(e.g. normal, hidden, archive, directory): ";
	while(!(cin >> nAttribute)){
		cout << "Invalid input. Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}
	
	DWORD newAttribute = AttributeDwordCreator(nAttribute);
	DWORD addedAttribute = currentAttribute | newAttribute;

	bool result = SetFileAttributesA(directory.c_str(), addedAttribute);
	return 0;
}
DWORD WINAPI DeleteFileAttribute(const string& directory) {
	
	string nAttribute;
	while (!(cin >> nAttribute)) {
		cout << "Unknown input. Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	DWORD newAttribute = AttributeDwordCreator(nAttribute);
	DWORD deletedAttribute = currentAttribute & ~newAttribute;

	bool result = SetFileAttributesA(directory.c_str(), deletedAttribute);
	if (result) {
		// сделать активным флаг изменения атрибута
	}
	return 0;
}

DWORD WINAPI ChangeAttributeObserver(LPVOID lpParams) {

	string* pointerDirectory = (string*)lpParams;
	string directory = *pointerDirectory;

	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];
	DWORD dwBytesReturned;

	HANDLE hDirectory = CreateFileA(directory.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	while (true) {
		if (ReadDirectoryChangesW(hDirectory, buffer, BUFFER_SIZE, FALSE, FILE_NOTIFY_CHANGE_ATTRIBUTES, &dwBytesReturned, NULL, NULL)) {
			FILE_NOTIFY_INFORMATION* pointerNotify = (FILE_NOTIFY_INFORMATION*)buffer;
			if (pointerNotify->Action == FILE_ACTION_MODIFIED) {
				cout << "NOTIFICATION: object attributes" << directory << "have been changed";
			}
		}
	}
	
	return 0;
}


int main()
{                                                                                                                                       
	int choice = -1;
	
	string directory;
	cout << "Write down directory which attribute you want to change: ";
	while (!(cin >> directory)) {
		cout << "Invalid input! Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	string* pointerData = new string(directory);

	hChangeAttributeObserverThread = CreateThread(NULL, 0, ChangeAttributeObserver, pointerData, 0, &dwChangeAttributeObserverId);

	while (true) {
		cout << "What do you want to do with this object?" << endl;
		cout << "1. Add attribute " << endl;
		cout << "2. Toggle attribute" << endl;
		cout << "3. Delete attribute " << endl;
		while(!(cin >> choice)){
			cout << "Invalid input! Try again";
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}
 		switch (choice) {
			case 1: {
				AddFileAttribute(directory.c_str());
				break;
			}
			case 2: {
				ToggleFileAttribute(directory.c_str());
				break;
			}
			case 3: {
				DeleteFileAttribute(directory.c_str());
				break;
			}
		}



	};

	delete pointerData;
	return 0;
}
