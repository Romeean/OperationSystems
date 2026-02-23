#include <iostream>
#include <windows.h>

using namespace std;

string GetRegionType(DWORD element) {
	switch (element) {
		case MEM_PRIVATE: {
			return "MEM_PRIVATE";
			break;
		}
		case MEM_MAPPED: {
			return "MEM_MAPPED";
			break;
		}
		case MEM_IMAGE: {
			return "MEM_IMAGE";
			break;
		}
		default: return "UNKNOWN";
	}
}

string GetState(DWORD element) {
	switch (element) {
		case MEM_COMMIT: {
			return "MEM_COMMIT";
			break;
		}
		case MEM_RESERVE: {
			return "MEM_RESERVED";
			break;
		}
		case MEM_FREE: {
			return "MEM_FREE";
			break;
		}
		
		default: return "UNKNOWN";
	}
}

string GetProtect(DWORD element) {
	switch (element) {
	case PAGE_READONLY: {
		return "PAGE_READONLY";
		break;
	}
	case PAGE_READWRITE: {
		return "PAGE_READWRITE";
		break;
	}
	case PAGE_EXECUTE: { 
		return "PAGE_EXECUTE";
		break;
	};
	case PAGE_EXECUTE_READ: {
		return "PAGE_EXECUTE_READ";
		break;
	};
	case PAGE_EXECUTE_READWRITE: {
		return "PAGE_EXECUTE_READWRITE";
		break;
	};
	case PAGE_NOACCESS: {
		return "PAGE_NOACCESS";
		break;
	}
	default:	return "UNKNOWN";
	}
}

int main()
{
	// FIRST PART
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	cout << "Page size: " << si.dwPageSize << endl;
	cout << "Smallest VAS: " << si.lpMinimumApplicationAddress << endl;
	cout << "Bigest VAS: " << si.lpMaximumApplicationAddress << endl;
	
	MEMORYSTATUSEX ms;
	ms.dwLength = sizeof(ms);

	GlobalMemoryStatusEx(&ms);

	cout << "Total memory copacity (RAM): " << ms.ullTotalPhys / (1024 * 1024 * 1024) << "GB" << endl;
	cout << "Available memory copacity (RAM): " << ms.ullAvailPhys / (1024 * 1024 * 1024) << "GB" << endl;
	cout << endl;
	cout << "Total memory copacity (TotalPageFile): " << ms.ullTotalPageFile / (1024 * 1024 * 1024) << "GB" << endl;
	//                                                            |
	//                                        значение которое отвечает за количество доступной памяти 
	//                                        систмеме = RAM + файлы подкачки
	cout << "Available memory copacity (TotalPageFile): " << ms.ullAvailPageFile / (1024ULL * 1024 * 1024 * 1024) << "GB" << endl;
	cout << endl;

	cout << "Total memory copacity (ullTotalVirtual): " << ms.ullTotalVirtual / (1024ULL * 1024 * 1024 * 1024) << "TB" << endl;
	//                                                            |
	//                                        объем логической памяти доступный каждому процессу, представленно как 
	//                                        сплошной массив, объемом 128 терабайт.
	cout << "Total copacity (Paging file): " << (ms.ullTotalPageFile - ms.ullTotalPhys) / (1024 * 1024 * 1024) << "GB" << endl;
 
	cout << endl;
	
	// SECOND PART  

	LPVOID currentAddress = si.lpMinimumApplicationAddress;
	LPVOID maximumAddress = si.lpMaximumApplicationAddress;

	MEMORY_BASIC_INFORMATION mbi;

	while (currentAddress < maximumAddress) {
		if (VirtualQuery(currentAddress, &mbi, sizeof(mbi)) == 0) break;
		
		cout << "Base address: " << mbi.BaseAddress << endl;
		cout << "Region size: " << mbi.RegionSize << endl;

		cout << "Region type: " << GetRegionType(mbi.Type) << endl;
		cout << "State: " << GetState(mbi.State) << endl;
		cout << "Protect: " << GetProtect(mbi.Protect) << endl;
		cout << endl;
		currentAddress = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
	}
	return 0;
}

