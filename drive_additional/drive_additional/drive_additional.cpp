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
	
	if (element == INVALID_FILE_ATTRIBUTES) return " Invalid attribute";


	if (element & FILE_ATTRIBUTE_READONLY) return " Readonly";
	if (element & FILE_ATTRIBUTE_HIDDEN) return " Hidden";
	if (element & FILE_ATTRIBUTE_SYSTEM) return " System";
	if (element & FILE_ATTRIBUTE_DIRECTORY) return " Directory";
	if (element & FILE_ATTRIBUTE_ARCHIVE) return " Archive";
	if (element & FILE_ATTRIBUTE_NORMAL) return " Normal";
	

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

void ToggleFileAttribute(const string& directory ) {

	string nAttribute;
	cout << "Enter attribute to toggle (readonly, hidden, archive, normal): ";
	while (!(cin >> nAttribute)) {
		cout << "Invalid input. Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}


	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	if (currentAttribute == INVALID_FILE_ATTRIBUTES) {
		cout << "Directive not found" << endl;
		return;
	}
	DWORD newAttribute = AttributeDwordCreator(nAttribute);
	DWORD toggledAttribute = currentAttribute ^ newAttribute;

	bool result = SetFileAttributesA(directory.c_str(), toggledAttribute);
	if (!result) {
		cout << "Failed to apply file attribute" << endl;
		return;
	}
	return;
}

void AddFileAttribute(const string& directory) {
	string nAttribute;

	
	cout << "Enter attribute to add (readonly, hidden, archive, normal): ";
	while(!(cin >> nAttribute)){
		cout << "Invalid input. Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	if (currentAttribute == INVALID_FILE_ATTRIBUTES) {
		cout << "Directive didn't found" << endl;
		return;
	}
	DWORD newAttribute = AttributeDwordCreator(nAttribute);
	DWORD addedAttribute = currentAttribute | newAttribute;

	bool result = SetFileAttributesA(directory.c_str(), addedAttribute);
	if (!result) {
		cout << "file attribute didn't apply" << endl;
		return;
	}
	return;
}

void DeleteFileAttribute(const string& directory) {
	
	string nAttribute;
	cout << "Enter attribute to delete (readonly, hidden, archive, normal): ";


	while (!(cin >> nAttribute)) {
		cout << "Invalid input. Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	if (currentAttribute == INVALID_FILE_ATTRIBUTES) {
		cout << "Directive didn't found" << endl;
		return;
	}
	DWORD newAttribute = AttributeDwordCreator(nAttribute);
	DWORD deletedAttribute = currentAttribute & ~newAttribute;

	bool result = SetFileAttributesA(directory.c_str(), deletedAttribute);
	if (!result) {
		cout << "file attribute didn't apply" << endl;
		return;
	}
	return;
}

DWORD WINAPI ChangeAttributeObserver(LPVOID lpParams) {

	string* pointerDirectory = (string*)lpParams;
	string directory = *pointerDirectory;

	size_t lastSlash = directory.find_last_of("\\/");
	string parentDirectory = directory.substr(0, lastSlash);

	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];
	DWORD dwBytesReturned;

	HANDLE hDirectory = CreateFileA(parentDirectory.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	while (true) {
		if (ReadDirectoryChangesW(hDirectory, buffer, BUFFER_SIZE, FALSE, FILE_NOTIFY_CHANGE_ATTRIBUTES, &dwBytesReturned, NULL, NULL)) {
			FILE_NOTIFY_INFORMATION* pointerNotify = (FILE_NOTIFY_INFORMATION*)buffer;
			if (pointerNotify->Action == FILE_ACTION_MODIFIED) {
				cout << "NOTIFICATION: object attributes " << directory << " have been changed" << endl;
			}
		}
	}
	return 0;
}


int main()
{                                                                                                                                       
	int choice = -1;
	
	string directory;
	cout << "Enter directory path: ";
	while (!(cin >> directory)) {
		cout << "Invalid input! Try again ";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	string* pointerData = new string(directory);

	hChangeAttributeObserverThread = CreateThread(NULL, 0, ChangeAttributeObserver, pointerData, 0, &dwChangeAttributeObserverId);

	while (true) {
		cout << '\n';
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
