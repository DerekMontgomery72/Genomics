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
	AppWrapper(string configFileName, string TextFileName);

	AppWrapper();

	~AppWrapper() { }

	void runApp();

	void testInputs();


private:

	vector<char> input;
	string alphabet;
	string sname;

};