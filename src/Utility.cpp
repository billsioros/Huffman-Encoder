// This is the Utility.cpp File:

#include "Utility.h"
#include "Huffman.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

static void _sleep(const unsigned int mls)
{
	usleep(mls * 1000);
	fflush(stdout);
}

static void _loading(const unsigned int mls, const unsigned int bar_size)
{
	for (unsigned int i = 0; i < bar_size + 1; i++) {
		cout << '[';
		for (unsigned int j = 0; j < bar_size; j++) {
			if (j < i)
				cout << '=';
			else if (j == i)
				cout << '>';
			else
				cout << ' ';
		}
		cout << ']' << ' ' << (100.0f / (float)bar_size) * (float)i << '%' << '\r';
		_sleep(mls);
	}
}

static bool legalWrite(const char* const filename)
{
	std::ifstream ifs(filename);
	if (!ifs.fail()) {
		std::cout << '\"' << filename << '\"' << " already exists! Do you want to overwrite it?" << std::endl;

		std::string input;
		do {
			std::cout << ">: ";
			std::cin >> input;

			if (input == "YES")
				return true;
			else if (input == "NO")
				return false;
			else
				std::cout << "<Invalid Input>" << std::endl;
		} while (true);
	}

	return true;
}

// Specific Functions:
void _calcfreq(float* const frequency, const char* const file, const bool _print)
{
	//Debug ios::binary
	ifstream ifs(file, ios::binary);
	if (ifs.fail()) {
		cerr << "<Fatal Error>: Unable to open file..." << endl;
		return;
	}

	ifs.seekg(0, ifs.end);
	const unsigned int file_size = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	for (unsigned int i = 0; i < 256; i++)
		frequency[i] = 0.0f;

	unsigned char ch;
	while (ifs >> noskipws >> ch) {
		for (unsigned int i = 0; i < 256; i++) {
			if (ch == (unsigned char) i) {
				frequency[i] += 1.0f / float(file_size);
				break;
			}
		}
	}

	if (_print) {
		cout << "<Frequencies>\n" << endl;

		for (unsigned int i = 0; i < 256; i++)
			if (frequency[i] > 0.0f)
				cout << (char)i << ' ' << frequency[i] << endl;

		cout << endl;
	}
}

void _detecode(const float* const frequency, string* const codes, const bool _print)
{
	Tree T(frequency);
	for (unsigned int i = 0; i < 256; i++) {
		const string current = T.search((unsigned char)i);
		if (!current.empty())
			codes[i] = current;
	}

	if (_print) {
		cout << "<Huffman Code>\n" << endl;

		for (unsigned int i = 0; i < 256; i++)
			if (!codes[i].empty())
				cout << (char)i << ' ' << codes[i] << endl;

		cout << endl;
	}
}

static void toBytes(const string& code_str, unsigned char* bytes, const unsigned int size)
{
	for (unsigned int i = 0; i < size; i++) {
		bytes[i] = 0x00;
		for (unsigned int j = 0; j < 8; j++) {
			unsigned int current = i * 8 + j;
			if (current < code_str.size() && code_str[current] == '1') {
				bytes[i] |= (0x01 << (7 - j));
			}
		}
	}
}

static void getFullCode(const char* const file, const string* const codes, string& _code)
{
	//Debug ios::binary
	ifstream ifs(file, ios::binary);
	if (ifs.fail()) {
		cerr << "<Fatal Error>: Unable to open file..." << endl;
		return;
	}

	unsigned char ch;
	while (ifs >> noskipws >> ch)
		_code += codes[(unsigned int)ch];
}

static void saveHdr(ofstream& ofs, const string* const codes)
{
	for (unsigned int i = 0; i < 256; i++)
		if (!codes[i].empty())
			ofs << (char)i << ' ' << codes[i] << endl;
}

static void saveDataBin(ofstream& ofs, const string& _code)
{
	const unsigned int size = _code.size() / 8 + (_code.size() % 8 ? 1 : 0);
	unsigned char* const bytes = new unsigned char[size];
	toBytes(_code, bytes, size);

	//Debug extra "###"
	ofs << "###" << endl << _code.size() << endl;
	for (unsigned int i = 0; i < size; i++)
		ofs << bytes[i];

	delete[] bytes;
}

void _compress(const char* const file)
{
	float frequency[256];
	_calcfreq(frequency, file);

	string codes[256];
	_detecode(frequency, codes);

	string _code;
	getFullCode(file, codes, _code);

	string output(file);
	const size_t pos = output.find_last_of('.');
	if (pos != string::npos)
		output = output.substr(0, pos);
	output += ".huff";

	if (!legalWrite(output.c_str()))
		return;

	ofstream ofs(output.c_str(), ios::trunc | ios::binary);
	if (ofs.fail()) {
		cerr << "<Fatal Error>: Unable to open file..." << endl;
		return;
	}

	saveHdr(ofs, codes);
	saveDataBin(ofs, _code);
	_loading(25, 20);
}

static void toString(string& code_str, const unsigned char* const bytes, const unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		for (unsigned int j = 0; j < 8; j++)
			code_str += (((bytes[i] & (0x01 << (7 - j))) != 0x00) + '0');
}

static void retrieveHdr(ifstream& ifs, string* const codes, unsigned int& max_code_size)
{
	string current;
	while (getline(ifs, current)) {
		if (!current.empty()) {
			const string _code = current.substr(2);
			const unsigned int letter = (unsigned int)current.substr(0, 1)[0];
			codes[letter] = _code;
			max_code_size = (max_code_size < _code.size() ? _code.size() : max_code_size);
		}
		else {
			break;
		}
	}
}

static void retrieveDataBin(ifstream& ifs, string& _code)
{
	unsigned int code_str_size;
	if (!ifs.eof())
		ifs >> code_str_size;
	ifs.ignore(9999, '\n');

	const unsigned int size = code_str_size / 8 + (code_str_size % 8 ? 1 : 0);
	unsigned char* const bytes = new unsigned char[size];

	unsigned int i = 0;
	unsigned char ch;
	while (ifs >> noskipws >> ch)
		bytes[i++] = ch;

	toString(_code, bytes, size);
	_code = _code.substr(0, code_str_size);
}

static void saveDataTxt(const char* const file, const string* const codes, const unsigned int max_code_size, const string& _code)
{
	ofstream ofs(file, ios::trunc);
	if (ofs.fail()) {
		cerr << "<Fatal Error>: Unable to open file..." << endl;
		return;
	}

	unsigned int i = 0;
	while (i < _code.size()) {
		for (unsigned int length = max_code_size; length >= 1; length--) {
			string mask = _code.substr(i, length);
			bool isDone = false;
			for (unsigned int k = 0; k < 256; k++)
				if (!codes[k].empty() && mask == codes[k]) {
					isDone = true;
					ofs << (char)k;
					break;
				}

			if (isDone) {
				i += length - 1;
				break;
			}
		}
		i++;
	}
}

void _decmprss(const char* const file)
{
	string filename(file);
	const string extnsn = filename.substr(filename.find_last_of('.'));
	if (extnsn != ".huff") {
		cerr << "<Fatal Error>: Wrong file type..." << endl;
		return;
	}

	ifstream ifs(file, ios::binary);
	if (ifs.fail()) {
		cerr << "<Fatal Error>: Unable to open file..." << endl;
		return;
	}

	string codes[256];
	unsigned int max_code_size = 0;
	retrieveHdr(ifs, codes, max_code_size);

	string _code;
	retrieveDataBin(ifs, _code);

	filename = filename.substr(0, filename.find_last_of('.'));
	filename += "_decomp.txt";

	if (!legalWrite(filename.c_str()))
		return;

	saveDataTxt(filename.c_str(), codes, max_code_size, _code);
	_loading(25, 20);
}

void _help()
{
	cout << "\n >COMMANDS<"																		<< endl;
	cout << "   -f  |  -frequency   : CALCULATE THE FREQUENCY OF EACH CHARACTER"				<< endl;
	cout << "   -h  |  -huffman     : FIND THE HUFFMAN REPRESENTATION OF EACH CHARACTER"		<< endl;
	cout << "   -c  |  -compress    : COMPRESS DATA BY MEANS OF HUFFMAN ENCODING"				<< endl;
	cout << "   -d  |  -decompress  : DECOMPRESS DATA BY MEANS OF HUFFMAN DECODING"				<< endl;

	cout << "\n >SYNTAX<"																		<< endl;
	cout << "  <command> \"filename\"\n"														<< endl;
}

// General Functions:
void _pause()
{
	cout << "> Press enter to continue... <" << endl;
	cin.ignore(9999, '\n');
	cin.clear();
	getchar();
}