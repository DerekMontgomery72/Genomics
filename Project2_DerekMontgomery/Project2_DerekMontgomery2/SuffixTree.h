#pragma once
#include<iostream>
#include <fstream>
#include<string>
#include"SuffixNode.h"

using namespace std;

class SuffixTree {
public:

	SuffixTree(vector<char> &s1, string alphabet);

	SuffixNode *findPath(SuffixNode* T, char* suff);

	SuffixNode* createTree(SuffixNode* T, char* suff);

	SuffixNode* NodeHop(SuffixNode* t, char* path, int pathLen, char* s);
	 
	void DFS();

	void DFSHelper(SuffixNode* t, int* maxRepeatLength, int* maxRepeatStart, ofstream *outfile);

	void BWT();

	void BWTHelper(SuffixNode* t, ofstream* outfile);

	
private:

	int getLenAlpha(SuffixNode* U);

	int getLenBetaPrime(SuffixNode* V, SuffixNode* Uprime);
	char* strFrontPtr;
	SuffixNode* root;
	int numNodes;
	int numLeafs;
	string alphabet;

};