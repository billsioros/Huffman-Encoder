// This is the main.cpp File:

#include "Utility.h"
#include <string>
#include <iostream>

using namespace std;

const unsigned int cmnd_index = 1;
const unsigned int file_index = 2;

int main(int argc, char *argv[])
{
	if (argc == 3) {
		const string file(argv[file_index]);
		const string cmnd(argv[cmnd_index]);

		if (cmnd == "-f" || cmnd == "-frequency") {
			float f[256];
			_calcfreq(f, file.c_str(), true);
		}
		else if (cmnd == "-h" || cmnd == "-huffman") {
			float f[256];
			_calcfreq(f, file.c_str());

			string codes[256];
			_detecode(f, codes, true);
		}
		else if (cmnd == "-c" || cmnd == "-compress") {
			_compress(file.c_str());
		}
		else if (cmnd == "-d" || cmnd == "-decompress") {
			_decmprss(file.c_str());
		}
		else {
			cerr << " ERROR: No valid command" << endl;
			return -1;
		}

		#ifndef __linux__
			_pause();
		#endif

		return 0;
	}
	else if (argc > 3) {
		cerr << " ERROR: Too many arguements"	<< endl;
	}
	else if (argc != 2) {
		cerr << " ERROR: Not enough arguements" << endl;
	}
	else {
		const string cmnd(argv[cmnd_index]);

		if (cmnd == "--h" || cmnd == "--help") {
			_help();
			#ifndef __linux__
				_pause();
			#endif

			return 0;
		}
		else {
			cerr << " ERROR: Not enough arguements" << endl;
		}
	}

	return -2;
}