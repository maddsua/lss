#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdbool.h>

#define FORMATSTRINGSIZE	(16)
#define LISTFILENAME 		L"ls.txt"

int main(int argc, char** argv) {
	
	_setmode(_fileno(stdout), _O_U16TEXT);	
	
	bool saveToFile = false;
	wchar_t formatFilter[FORMATSTRINGSIZE];
	memset(formatFilter, 0, sizeof(wchar_t) * FORMATSTRINGSIZE);

	//	deal with arguments
	if (argc > 1) {
		
		for (int i = 1; i < argc; i++) {
			
			if (!strcmp(argv[i], "-s")) {
				saveToFile = true;
				continue;

			} else if (argv[i][0] == '.' && strlen(argv[i]) < FORMATSTRINGSIZE) {
				mbstowcs(formatFilter, argv[i], FORMATSTRINGSIZE);
				continue;
			}
		}
	}
	
	FILE* listFile;
	wchar_t dirPath[MAX_PATH];

	//	do we save listing?
	if (saveToFile) {
		
		listFile = _wfopen(LISTFILENAME, L"wt, ccs=UTF-16LE");
		
		if (listFile != NULL){
			
			GetCurrentDirectoryW(MAX_PATH, dirPath);
			
			if (wcslen(formatFilter) == 0) {
				fwprintf(listFile, L"Listing of %s:\n\n", dirPath);
			} else {
				fwprintf(listFile, L"Listing of %s (%s files):\n\n", dirPath, formatFilter);
			}

		} else {
			saveToFile = false;
		}
	}
	
	//	find files in current dir and list them
	WIN32_FIND_DATAW data = {0};
    HANDLE hFind = FindFirstFileW(L"*.*", &data);

	//	enable console color settings
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
		
	//	find files
    if (hFind != INVALID_HANDLE_VALUE) {
    	
    	wprintf(L"\n");
    	
		while (FindNextFileW(hFind, &data)) {
			
			//	excluse list file
			if (wcsstr(data.cFileName, LISTFILENAME) != NULL){
				continue;
			}
			
			if ((wcslen(formatFilter) == 0 && wcsstr(data.cFileName, L"..") == NULL) || (wcslen(formatFilter) > 0 && wcsstr(data.cFileName, formatFilter) != NULL)){
				
				if (data.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY) {
					
					SetConsoleTextAttribute(hConsole, 10);
					wprintf(L"%s\n",  data.cFileName);
					SetConsoleTextAttribute(hConsole, 7);
				} else {
					wprintf(L"%s\n",  data.cFileName);
				}
				
				if (saveToFile) {

					if (data.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY) {
						fwprintf(listFile, L"[dir] %s\n", data.cFileName);
					} else {
						fwprintf(listFile, L"%s\n", data.cFileName);
					}
				}
			}
		}
		
		FindClose(hFind);
    } else {
    	wprintf(L">> WIN32API ERROR!");
	}
    
    //	close file
    if (saveToFile) {
    	wprintf(L"\n>> Saved to ls.txt\n");
    	fclose(listFile);
	}
	
	return 0;
}

