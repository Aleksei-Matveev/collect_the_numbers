#include "stdafx.h"

void saveDisk(){
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<TCHAR>);
	std::wofstream ofile;
	ofile.imbue(loc);
	ofile.open("data.db");
	if (!ofile.is_open())
		MessageBox(hDialogGame, _TEXT("ERROR SAVE"), NULL, MB_OK);
	if (!Table.empty()) {
		auto itfile = Table.rbegin();
		for (int i = 0; i < 10; i++) {
			for (UINT i = 0; i < _tcslen(itfile->second); i++) {
				if (itfile->second[i] == ' ')
					itfile->second[i] = '_';
			}
			ofile << itfile->second << ' ' << itfile->first << '\n';
			itfile++;
		}
	}
	ofile.close();
}

void loadDisk() {
	static TCHAR str[] = _TEXT("NONAME");
	TCHAR* ptr;
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<TCHAR>);
	std::wifstream ifile;
	ifile.imbue(loc);
	fileBuffer = new TCHAR*[10];
	ifile.open("data.db", std::ios::binary);
	if (ifile.is_open()) {
		for (UINT i = 0; i < 10; i++) {
			fileBuffer[i] = new TCHAR[19];
			ifile >> fileBuffer[i] >> totalPoints;
			ptr = fileBuffer[i];
			for (UINT j = 0; j < _tcslen(ptr); j++) {
				if (ptr[j] == '_')
					ptr[j] = ' ';
			}
			Table.insert(std::pair<int, TCHAR*>(totalPoints, fileBuffer[i]));
		}
	}
	else {
		for (int i = 0; i < 10; i++) {
			fileBuffer[i] = new TCHAR[20];
			_tcscpy(fileBuffer[i], str);
			Table.insert(std::pair<int, TCHAR*>(i * 10, fileBuffer[i]));
		}
	}
	ifile.close();
}