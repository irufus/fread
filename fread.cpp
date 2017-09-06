#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

/*
	getFileSize
	Function that is used to get the file size length of the specified file
*/
size_t getFileSize(const char* fileName) {
	struct stat st;
	if (stat(fileName, &st) != 0) {
		return 0;
	}
	return st.st_size;
}

int main(int argc, char* argv[])
{
	bool isWatching;
	isWatching = false;
	if (argc < 3) {
		/*
			Options:
			-s - File stream reading (supported)
			-t - Reads the last line of the file. The tail end.
		*/
		std::cout << "Usage is -<option> <file>\n-s - File streaming\n-t - Only read the tail\n\n";
		exit(0);
	}
	else {
		std::ifstream inFile;
		char* myFile = argv[2];

		std::cout << argv[0] << "\n";
		if (std::string(argv[1]) == "-s")
			isWatching = true;
		
		inFile.open(myFile, std::ios_base::binary); //windows files require a binary read to prevent duplicate line breaks \r\n
		if (!inFile) {
			std::cerr << "Unable to open: " << myFile << "\n";
			exit(1);
		}
		else {
			int length = 0, oldLength = 0;
			do {
				length = getFileSize(myFile);
				if (length != oldLength && length > 0) {
					bool hasReadNewLine = false;
					for (int i = length - 2; i > 0; i--) {
						inFile.seekg(i);
						char c = inFile.get();							
						if (c == '\r' || c == '\n') {
							if (hasReadNewLine) {
								oldLength = i;
								break;
							}
							else {
								hasReadNewLine = true;
							}
						}
					}
					char * s = new char[length - oldLength];
					inFile.get(s, length - oldLength);
					if (inFile)
						std::cout << s << "\n";
					else{
						std::cout << "Unable to read " << oldLength << " to " << length;
						length = 0;
					}

					oldLength = length;
					delete[] s;
				}
				_sleep(1);
			} while (length && isWatching);
			inFile.close();
		}
	}
    return 0;
}

