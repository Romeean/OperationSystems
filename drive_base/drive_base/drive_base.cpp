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
		return " network";
		break;
	}
	case 4: {
		return " CDROM/DVD";
		break;
	}
	case 5: {
		return " RAM";
		break;
	}
	default: {
		return "Unknown input";
		break;
	}
	}
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
		if (!(cin >> choice)) {
			cout << "Unknown command \n";
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}

		switch (choice) {
		
		case 1: {
			const int LETTERS = 26;

			DWORD mask = GetLogicalDrives();
			for (int i = 0; i < LETTERS; i++) {
				int result = mask & (1 << i);
				
				if (result) {
					cout << "Disk " << DriveNameEncode(i) <<" exists" << endl;
				}
			}

			char letter;
			cout << "Write the desired drive to check the type";
			cin >> letter;
			
			string drivePath = "";
			drivePath += letter;
			drivePath += ":\\";

			int type = GetDriveTypeA(drivePath.c_str());

			string result = DriveTypeEncode(type);

			cout << "disk " << letter << ": " << result;

			break;
		};
		case 2: {

		}
		case 3: {

		}
		case 4: {

		}
		}
	}


}