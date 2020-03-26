#include "AppWrapper.h"


AppWrapper::AppWrapper(string alphabetFileName, string TextFileName) {
	ifstream config, inputStringFile;
	string line;
	std::locale loc;
	config.open(alphabetFileName);
	inputStringFile.open(TextFileName);

	if (config.is_open() && inputStringFile.is_open()) {
		while (getline(config, line))
		{
			for (int i = 0; i < line.length(); i++) {
				if (islower(line[i], loc)) {
					cout << "The alphabet cannot have lower case characters" << endl;
					throw new exception("The input alphabet cannot have lowercase letters");
				}
				if (line[i] != ' ' && line[i] != '\n')
				{
					this->alphabet.push_back(line[i]);
				}
			}
		}
		while (getline(inputStringFile, line))
		{
			if (line[0] != '>') {

				line.pop_back();
				for (int i = 0; i < line.length(); i++) {
					this->input.push_back(line[i]);
				}
			}
			else {
				line.erase(0, 1);
				this->sname = line;
			}
		}

		this->input.push_back('$');
		this->input.push_back('\0');
	}
	else {
		cout << "There Was an issue while opening the input files" << endl;
	}
}

AppWrapper::AppWrapper() 
{
	ifstream config, inputStringFile;
	string line;
	std::locale loc;
	string alphabetFilename, inputTextName;
	cout << "Please give the alphabet file name" << endl;
	getline(cin, alphabetFilename);
	cout << "Please give the FASTA file name" << endl;
	getline(cin, inputTextName);

	
	config.open(alphabetFilename);
	inputStringFile.open(inputTextName);

	if (config.is_open() && inputStringFile.is_open()) {
		while (getline(config, line))
		{
			for (int i = 0; i < line.length(); i++) {
				if (islower(line[i], loc)) {
					cout << "The alphabet cannot have lower case characters" << endl;
					throw new exception("The input alphabet cannot have uppercase letters");
				}
				if (line[i] != ' ' && line[i] != '\n')
				{
					this->alphabet.push_back(line[i]);
				}
			}
		}
		while (getline(inputStringFile, line))
		{
			if (line[0] != '>') {

				for (int i = 0; i < line.length(); i++) {
					this->input.push_back(line[i]);
				}
			}
			else {
				line.erase(0, 1);
				this->sname = line;
			}
		}
		this->input.push_back('$');
		this->input.push_back('\0');
	}
	else {
		cout << "There Was an issue while opening the input files" << endl;
	}
}

void AppWrapper::runApp()
{
	clock_t start, end;
	start = clock();
	SuffixTree sT = SuffixTree(this->input, this->alphabet);
	end = clock();

	double duration = double(end - start) / double(CLOCKS_PER_SEC);

	cout << "Tree construction time:  " << duration << " seconds" << endl;

	sT.DFS();
	sT.BWT();
	return;
}

void AppWrapper::testInputs() {
	cout << "*********************************************" << endl;
	cout << "* ********** input file tests ***************" << endl;
	cout << "input Alphabet" << endl << this->alphabet << endl;
	cout << "input string name: " << this->sname << endl;
	cout << "input string size: " << this->input.size() << endl;
	cout << "input string: ";
	for (int i = 0; i < (input.size() / sizeof(char)); i++) {
		cout << input[i];
	}
	cout << endl;
	return;
}