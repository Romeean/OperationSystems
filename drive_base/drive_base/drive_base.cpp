#include <iostream>
#include <Windows.h>

using namespace std;


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