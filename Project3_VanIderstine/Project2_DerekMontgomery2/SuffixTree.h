#pragma once

#include<iostream>
#include <fstream>
#include<string>
#include"SuffixNode.h"

using namespace std;

class SuffixTree {
public:

	SuffixTree(vector<char> &s1, vector<char>& s2, string alphabet, string alphabet2);

	SuffixNode *findPath(SuffixNode* T, char* suff, int color);

	SuffixNode* createTree(SuffixNode* T, char* suff, int color);

	SuffixNode* NodeHop(SuffixNode* t, char* path, int pathLen, char* s, int color);
	 
	void DFS();

	void colorNodes(SuffixNode *U);

	void DFSHelper(SuffixNode* t, int* maxRepeatLength, int* maxRepeatStart, ofstream *outfile);

	void BWT();

	void BWTHelper(SuffixNode* t, ofstream* outfile);

	vector<char> findFingerprint(SuffixNode* U, int color);

	
private:

	int getLenAlpha(SuffixNode* U);

	int getLenBetaPrime(SuffixNode* V, SuffixNode* Uprime);
	char* strFrontPtr;
	SuffixNode* root;
	int numNodes;
	int numLeafs;
	string alphabet;

};