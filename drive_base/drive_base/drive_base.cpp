#include <iostream>
#include <Windows.h>

using namespace std;

char DriveNameEncode(int element) {
	switch (element) {
	case 0:  return 'A';
	case 1:  return 'B';
	case 2:  return 'C';
	case 3:  return 'D';
	case 4:  return 'E';
	case 5:  return 'F';
	case 6:  return 'G';
	case 7:  return 'H';
	case 8:  return 'I';
	case 9:  return 'J';
	case 10: return 'K';
	case 11: return 'L';
	case 12: return 'M';
	case 13: return 'N';
	case 14: return 'O';
	case 15: return 'P';
	case 16: return 'Q';
	case 17: return 'R';
	case 18: return 'S';
	case 19: return 'T';
	case 20: return 'U';
	case 21: return 'V';
	case 22: return 'W';
	case 23: return 'X';
	case 24: return 'Y';
	case 25: return 'Z';
	default: return 0; 
	}
}

string DriveTypeEncode(int element) {
	switch (element) {
	case 0: {
		return " unknown";
		break;
	}
	case 1: {
		return " no root directory";
		break;

	}
	case 2: {
		return " removable";
		break;
	}
	case 3: {
		return " fixed";
		break;
	}
	case 4: {
		return " remote";
		break;
	}
	case 5: {
		return " CDROM/DVD";
		break;
	}
	case 6: {
		return " RAM";
		break;
	}
	default: {
		return "Unknown input";
		break;
	}
	}
}


void GetLogicalDrivesCustom() {
	const int LETTERS = 26;

	DWORD mask = GetLogicalDrives();
	for (int i = 0; i < LETTERS; i++) {
		int result = mask & (1 << i);

		if (result) {
			cout << "Disk " << DriveNameEncode(i) << " exists" << endl;
		}
	}
}
void GetDriveTypeCustom(char letter) {

	string drivePath = "";
	drivePath += letter;
	drivePath += ":\\";

	int type = GetDriveTypeA(drivePath.c_str());

	string result = DriveTypeEncode(type);
	cout << "disk " << letter << ": " << result << endl;

}

void GetDiskFreeSpaceExCustom(char letter) {
	ULARGE_INTEGER FreeBytesAvailableToCaller;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;

	string path = "";
	path += letter;
	path += ":\\";

	bool result = GetDiskFreeSpaceExA(path.c_str(),
		&FreeBytesAvailableToCaller,
		&TotalNumberOfBytes,
		&TotalNumberOfFreeBytes
	);

	if (result) {
		cout << "Free bytes available to caller: " << FreeBytesAvailableToCaller.QuadPart / (1024 * 1024 * 1024) << "GB" << endl;
		cout << "Total numbers of bytes: " << TotalNumberOfBytes.QuadPart / (1024 * 1024 * 1024) << "GB" << endl;
		cout << "Total numbers of free bytes: " << TotalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024) << "GB" << endl;

	}
	else {
		cout << "There must be a problem with your input or drive " << letter << " doesn't exist." << endl;
	}
}

void GetSystemDirectoryCustom() {
	char buffer[MAX_PATH];
	UINT size = GetSystemDirectoryA(buffer, MAX_PATH);

	if (size && size < MAX_PATH) {
		cout << "System directory: " << buffer << endl;
	}
	else {
		cout << "There is defenetly a problem with your computer" << endl;
	}
	
}
void GetCurrentDirectoryCustom() {
	char buffer[MAX_PATH];
	UINT size = GetCurrentDirectoryA(MAX_PATH, buffer);

	if (size && size < MAX_PATH) {
		cout << "Windows directory: " << buffer << endl;
	}
	else {
		cout << "There is defenetly a problem with your computer" << endl;

	}

}
string FileAttributesEncode(int element) {
	switch (element) {
	case -1: return "Invalid";
	case 1: return "Readonly";
	case 2: return "Hidden";
	case 4: return "System";
	case 16: return "Directory";
	case 32: return "Archive";
	case 128: return "Normal";
	case 256: return "Temporary";
	case 2048: return "Compressed";
	case 4096: return "Ofline";
	
	}
}

void GetFilesAttributesExCustom(const string& pathAttributes){

	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	bool result = GetFileAttributesExA(pathAttributes.c_str(), GetFileExInfoStandard, &fileInfo);

	if (result) {

		ULARGE_INTEGER time;
		time.LowPart = fileInfo.ftCreationTime.dwLowDateTime;
		time.HighPart = fileInfo.ftCreationTime.dwHighDateTime;

		SYSTEMTIME stCreatedAt;
		FileTimeToSystemTime(&fileInfo.ftCreationTime, &stCreatedAt);

		SYSTEMTIME stLastTouched;
		FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &stLastTouched);

		SYSTEMTIME stLastWriteTime;
		FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &stLastWriteTime);

		uint64_t realFileSize = ((uint64_t)fileInfo.nFileSizeHigh << 32) | fileInfo.nFileSizeLow;


		cout << "\n";
		cout << "information for path: " << pathAttributes << endl;
		cout << "attributes: " << FileAttributesEncode(fileInfo.dwFileAttributes) << endl;
		cout << "created at: " << stCreatedAt.wDay << "." << stCreatedAt.wMonth << "." << stCreatedAt.wYear << endl;
		cout << "last touched at: " << stLastTouched.wDay << "." << stLastTouched.wMonth << "." << stLastTouched.wYear << endl;
		cout << "last write time at: " << stLastWriteTime.wDay << "." << stLastWriteTime.wMonth << "." << stLastWriteTime.wYear << endl;
		cout << "total file size: " << realFileSize << " bytes" << endl;

	}
}

void GetFileExtensionCustom(const string& directory, const string& extension ) {
	
	string path = directory + "\\*" + extension;
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(path.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE) {
		cout << "files with " << extension << " doesn't exist";
		return;
	}

	cout << "files in " << directory << " with " << extension << " extension" << endl;
	cout << "found " << findData.cFileName << endl;

	while (FindNextFileA(hFind, &findData)) {
		cout << "found " << findData.cFileName << endl;
	}
	
	FindClose(hFind);
}

int main()
{
	int choice = -1;

	while (true) {
		cout << "\n";
		cout << "1. Information about drives" << endl;
		cout << "2. Information about Operation System" << endl;
		cout << "3. All file's attributes in specific directory" << endl;
		cout << "4. All files with .txt extension" << endl;
		cout << "\n";

		cout << "Your choice: ";

		while (!(cin >> choice)) {
			cout << "Unknown command \n";
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}


		switch (choice) {
		
		case 1: {
			GetLogicalDrivesCustom();
			
			// --- //

			char letterDriveType;
			cout << "Enter drive letter (e.g. c, d, e): ";

			while (!(cin >> letterDriveType) || !isalpha(letterDriveType)) {
				cout << "Invalid input. Please enter a SINGLE letter: ";
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			GetDriveTypeCustom(letterDriveType);

			// --- //

			char letterFreeSpace;
			cout << "Enter drive letter (e.g. c, d, e): ";

			while (!(cin >> letterFreeSpace) || !isalpha(letterFreeSpace)) {
				cout << "Invalid input. Please enter a SINGLE letter: ";
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			GetDiskFreeSpaceExCustom(letterFreeSpace);

			cout << endl;
			break;
		};
		case 2: {
			GetSystemDirectoryCustom();
			GetCurrentDirectoryCustom();

			break;
		}
		case 3: {
			string pathAttributes;
			cout << "Enter the directory path (e.g. C:\Windows\System32): ";
			while (!(cin >> pathAttributes)) {
				cout << "There must be a problem with input. Try again" << endl;
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			GetFilesAttributesExCustom(pathAttributes);
			break;
		}
		case 4: {
			string directory;
			cout << "Enter directory path (e.g C:\Windows): ";
			while (!(cin >> directory)) {
				cout << "Unknown input, try again" << endl;
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			
			string extension;
			cout << "Enter file extension (e.g .tsx, .exe, .txt): ";
			while (!(cin >> extension)) {
				cout << "Invalid input, please try again" << endl;
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			GetFileExtensionCustom(directory, extension);
		}
		}
	}


}