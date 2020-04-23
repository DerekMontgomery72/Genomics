#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <locale>
#include <vector>
#include <chrono>
#include "SuffixTree.h"

using namespace std;

class AppWrapper {
public:
	AppWrapper(string configFileName, string TextFileName, string alphabetFileName2, string TextFileName2);

	AppWrapper();

	~AppWrapper() { }

	void runApp();

	void testInputs();


private:

	vector<char> input, input2;
	string alphabet, alphabet2;
	string sname, sname2;

};