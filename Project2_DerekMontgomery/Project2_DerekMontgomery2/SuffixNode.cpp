#include "SuffixNode.h"


SuffixNode::SuffixNode( SuffixNode* parent, int ID, int depth, int leafIndex, char* startPtr, SuffixNode* SuffLink)
{
	this->ID = ID;
	this->stringDepth = depth;
	this->leafIndex = leafIndex;
	this->parent = parent;
	this->startPtr = startPtr;
	this->suffLink = SuffLink;
	this->children = vector<SuffixNode*>();

}


void SuffixNode::printParentEdgeLabel() {
	
	char* cp;
	cp = this->startPtr;
	for (int i = 0; i < this->stringDepth; i++)
	{
		cout << (*this->startPtr);
		cp++;
	}

}

int SuffixNode::getID() {
	return this->ID;
}

int SuffixNode::getDepth() {
	return this->stringDepth;
}

SuffixNode* SuffixNode::getLink() {
	return this->suffLink;
}

SuffixNode* SuffixNode::getParent() 
{
	return this->parent;
}

vector<SuffixNode*>* SuffixNode::getChildrenPointers() 
{
	vector<SuffixNode*>* rv = &(this->children);
	return rv;
}

char* SuffixNode::getLinkLabel() 
{
	return this->startPtr;
}

void SuffixNode::setDepth(const int newDepth)
{
	this->stringDepth = newDepth;
}

void SuffixNode::setParent(SuffixNode* p)
{
	this->parent = p;
}

void SuffixNode::setLink(SuffixNode* newLink)
{
	this->suffLink = newLink;
}


void SuffixNode::setChildren(vector<SuffixNode*>* children) {
	this->children = *children;
}

void SuffixNode::setString(char* newStart, const int newDepth)
{
	this->startPtr = newStart;

	this->stringDepth = newDepth;

}
int SuffixNode::getLeafIndex()
{
	return this->leafIndex;
}