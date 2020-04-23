#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "dpCell.h"

using namespace std;

class Align {
public:

	Align();

	~Align();

	int globalAlign();

	int scoreChar(char ai, char bj);

	int Max(int s1, int s2, int s3);

	int maxLocal(int s1, int s2, int s3);

	int maxInsertion(DpCell** table, int i, int j);

	int maxDeletion(DpCell** table, int i, int j);

	int maxSubstitution(DpCell** table, int i, int j, char ai, char bj);
	 
	string walkBack(DpCell** table, int endI, int endJ);

	int scoreChar(char ai, char bj);

private:

	iostream Infile;
	string s1, s2;
	int match;
	int mismatch;
	int gapOpen;
	int gapCont;
	

};