#pragma once
#include <iostream>
#include <string>
#include<vector>


using namespace std;

class SuffixNode {
public:
	SuffixNode(SuffixNode* parent, int ID, int depth, int leafIndex, char* startPtr, SuffixNode* SuffLink);
	
	~SuffixNode() {};

	void printParentEdgeLabel();

	///Getters
	int getID();
	
	int getDepth();

	SuffixNode* getLink();

	SuffixNode* getParent();

	vector<SuffixNode*>* getChildrenPointers();

	char* getLinkLabel();

	int getLeafIndex();

	//Setters

	void setDepth(const int newDepth);

	void setParent(SuffixNode* p);

	void setLink(SuffixNode* newLink);

	void setChildren(vector<SuffixNode*>* children);

	void setString(char* newStart, const int newDepth);

private:

	int ID;
	int stringDepth;
	int leafIndex;
	SuffixNode* suffLink;
	SuffixNode* parent;
	vector<SuffixNode *> children;
	char *startPtr;

};