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



	DWORD addedAttribute = currentAttribute | newAttribute
	return 0;
}
DWORD WINAPI DeleteFileAttribute(const string& directory) {

	return 0;
}
DWORD WINAPI ChangeAttributeObserver(LPVOID lpParams) {



	return 0;
}

DWORD WINAPI ChangeFileAttributes(LPVOID lpParams) {

	string directory;
	cout << "Write down directory which attribute you want to change: ";
	while (!(cin >> directory)) {
		cout << "Incorrect input! Try again: ";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}
	DWORD currentAttribute = GetFileAttributesA(directory.c_str());
	string result = OutputAttributeEncode(currentAttribute);
	cout << "path " << directory << " has " << ": " << result;



	string newAttributeName;
	cout << "Which attribute you want to set up(e.g. directory, hidden, normal):";
	while (!(cin >> newAttributeName)) {
		cout << "Incorrect input! Try again";
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	DWORD additionalAttribute = AttributeDwordCreator(newAttributeName);


	DWORD newAttribute = currentAttribute | additionalAttribute;
	SetFileAttributesA(directory.c_str(), newAttribute);

	DWORD updatedAttribute = GetFileAttributesA(directory.c_str());

	cout << "updated attribute for " << directory << ": " << OutputAttributeEncode(updatedAttribute);


	return 0;
}

DWORD WINAPI OutputAttributesChange(LPVOID lpParams) {
	return 0;
}



int main()
{
	int choice = -1;
	hChangeAttributeObserverThread = CreateThread(NULL, 0, ChangeAttributeObserver, NULL, 0, &dwChangeAttributeObserverId);

	while (true) {

		string directory;
		cout << "Write down directory which attribute you want to change: ";
		while (!(cin >> directory)) {
			cout << "Invalid input! Try again";
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}

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

				break;
			}
			case 2: {
				break;
			}
			case 3: {
				break;
			}
		}



	};


	return 0;
}
