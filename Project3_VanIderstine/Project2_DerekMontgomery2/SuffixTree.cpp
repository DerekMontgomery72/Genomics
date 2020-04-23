#include "SuffixTree.h"


SuffixTree::SuffixTree(vector<char>& s1, vector<char>& s2, string alphabet, string alphabet2)
{
	vector<char> fingerprint;
	this->numNodes = 0;
	this->numLeafs = 0;
	//Change the vector over to a char* and null terminate it
	char* lineAsChar = &s1[0], *cp;
	char* lineAsChar2 = &s2[0];
	char* alph = &alphabet[0];
	char* alph2 = &alphabet2[0];
	this->alphabet = alph;
	this->strFrontPtr = lineAsChar;
	cp = lineAsChar;
	//SuffixNode* lastInserted = NULL;
	// Initialize root
	SuffixNode* root = new SuffixNode(NULL, 0, 0, 0, NULL, NULL);
	root->setColor(3);
	root->setParent(root);
	root->setLink(root);

	this->root = root;

	createTree(this->root, cp, 1);
	cp = lineAsChar2;
	createTree(this->root, cp, 2);
	colorNodes(this->root);
	fingerprint = findFingerprint(this->root, 1);
	cout << "First fingerprint: ";
	for (int i = 0; i < fingerprint.size(); i++)
	{
		cout << fingerprint[i];
	}
	cout << endl;
	fingerprint = findFingerprint(this->root, 2);
	cout << "Second fingerprint: ";
	for (int i = 0; i < fingerprint.size(); i++)
	{
		cout << fingerprint[i];
	}
	cout << endl;

	/*
	//Naive Algorithm for testing purposes
	for (int i = 0; i <= strlen(lineAsChar); i++) {
		//Insert suffix into tree
		if (this->root == NULL) // No root set yet
		{
			continue;
		}
		else
		{
			lastInserted = this->findPath(this->root, cp);
			cp++;
		}
	}
	*/

}

SuffixNode* SuffixTree::findPath(SuffixNode* T, char* suff, int color) 
{
	int i = 0, j = 0;
	vector<SuffixNode*>* TChildren = NULL;

	// T has no children -- allocate children list and set to T's children
	if (T->getChildrenPointers() == NULL) 
	{
		SuffixNode* newChild = new SuffixNode(T, this->numNodes + 1, strlen(suff), this->numLeafs + 1, suff, NULL);
		newChild->setColor(color);
		this->numLeafs += 1;
		this->numNodes += 1;
		vector<SuffixNode*>* newChildList = new vector<SuffixNode*>();
		newChildList->push_back(newChild);
		T->setChildren(newChildList);
		return newChild;
	}

	TChildren = T->getChildrenPointers();
	char* suffPtr = suff;
	char* parentLinkPtr = NULL;
	//Case == Iterate through children and compare first character of suffix link
	for (i = 0; i < T->getChildrenPointers()->size(); i++) 
	{
		SuffixNode* child = TChildren->at(i);

		if (*(child->getLinkLabel()) == *suff) //First Character Matches
		{
			parentLinkPtr = child->getLinkLabel();
			for (j = 0; (j < strlen(suff)) && (j < child->getDepth()) ; j++)
			{
				if (suffPtr[j] == parentLinkPtr[j]) // match
				{
					continue;
				}
				else // mismatch -- enter new suffix into tree where i is depth and suff[i] is new start for child
				{
					char* newSuffStart = suffPtr + j;
					char* newLinkStart = parentLinkPtr + j;


					SuffixNode* newLeaf = new SuffixNode(NULL, this->numNodes + 1, strlen(newSuffStart), this->numLeafs + 1, newSuffStart, NULL);
					this->numNodes += 1;
					this->numLeafs += 1;

					newLeaf->setColor(color);

					SuffixNode* newNode = new SuffixNode(T, this->numNodes + 1,j, 0, parentLinkPtr, NULL);
					this->numNodes += 1;

					newLeaf->setParent(newNode);
					newNode->setColor(color);

					int newDepth = child->getDepth() - j;

					child->setString(newLinkStart, newDepth);
					child->setParent(newNode);

					vector<SuffixNode*>* newNodeChildren = newNode->getChildrenPointers();

					if (*newSuffStart == '$' || *newSuffStart < *newLinkStart)
					{
						newNodeChildren->push_back(newLeaf);
						newNodeChildren->push_back(child);
					}
					else
					{
						newNodeChildren->push_back(child);
						newNodeChildren->push_back(newLeaf);
					}
					TChildren->erase(TChildren->begin() + i);
					

					//Insert the new cut link into the tree under the original parent
					vector<SuffixNode*>::iterator it;
					char* temp;
					it = TChildren->begin();
					
					while (it != TChildren->end())
					{
						temp = (*it)->getLinkLabel();
						if (*suff == '$' || *suff < *temp)
						{
							TChildren->insert(it, newNode);
							return newLeaf;
						}
						else
						{
							it++;
						}
					}

					TChildren->push_back(newNode);
					return newLeaf;

				} // end else
			} // end for loop iterating through characters

			if (j == strlen(suffPtr)) //Suffix used up is already in tree 
			{
				//cout << "This suffix has already been entered" << endl;
				return child;
			}

			else if (j == child->getDepth()) //Node link used -- need to recursively iterate further into the tree
			{
				char* remainingSuff = suff + (j);

				SuffixNode *newLeaf = findPath(child, remainingSuff, color);
				return newLeaf;
			}
		}
		
	}
	// Case iterated through full list of children -- no suffixes start with same character
	// insert new leaf under T with link of the full suffix
	char* temp;
	SuffixNode* newLeaf = new SuffixNode(T, this->numNodes + 1, strlen(suff), this->numLeafs + 1, suff, NULL);

	newLeaf->setColor(color);
	this->numNodes += 1;
	this->numLeafs += 1;
	vector<SuffixNode*>::iterator it;
	it = TChildren->begin();
	while (it != TChildren->end())
	{
		SuffixNode* link = *it;
		temp = link->getLinkLabel();
		if (*temp == '$' || *suff < *temp) // insert in middle
		{
			TChildren->insert(it, newLeaf);
			return newLeaf;
		}
		else
		{
			it++;
		}
	}
	// Insert on end
	TChildren->push_back(newLeaf);
	return newLeaf;
}

SuffixNode* SuffixTree::createTree(SuffixNode* U, char* suff, int color) // insertion but now with suffix links -- assume root already created and first node passed
{
	SuffixNode* localU = U, *temp = NULL, *UPrime, *Vprime, *V;
	char* cp = suff, *beta, *pathToFind, *betaPrime;
	int lenAlpha = 0, betaLength = 0, pathLen = 0;
	

	for (int i = 0; i < strlen(suff); i++)
	{

		if (i == 370)
		{
			// testing point where it is currently breaking
			int t = 0;
		}
		//Case 1
		if (localU->getLink() != NULL) // Suffix Link of U is known
		{
			V = localU->getLink();
			if (localU != this->root) // Case 1A SL(U) is known and u is not the root
			{
				char* endOflinkToU = localU->getLinkLabel() + localU->getDepth();

				lenAlpha = this->getLenAlpha(localU) - 1;
				pathToFind = (&(cp[i])) + lenAlpha;
				
				V = localU->getLink();

				// This case would be simpler with start index and end index storage, but I am storing the starting pointer of the string and its length
				// instead of the indexes
				//findPath(V, pathToFind);
				temp = findPath(V, pathToFind, color);
				localU = temp->getParent();
			}
			else //Case 1B SL(U) is known and U is the root
			{
				temp = findPath(this->root, &(cp[i]), color);
				localU = temp->getParent();
			}
		}
		//Case 2 SL(U) is unknown
		else
		{
			if (localU->getParent() != this->root) // Case 2A SL(U) is unknown and U' is not the root
			{
				UPrime = localU->getParent();
				beta = localU->getLinkLabel();
				betaLength = localU->getDepth();
				lenAlpha = this->getLenAlpha(localU) - 1;

				Vprime = UPrime->getLink();
				if (Vprime == NULL)
				{
					Vprime = UPrime->getParent()->getLink();
				}


				pathToFind = &cp[i] + (lenAlpha - betaLength);

				V = NodeHop(Vprime, beta, betaLength, pathToFind, color);
				localU->setLink(V);
				// Need to find the length of Alpha

				pathToFind = &cp[i] + lenAlpha;
				
				temp = findPath(V, pathToFind, color);
				localU = temp->getParent();

			}
			else // Case 2B SL(U) is unkown and U' is the root
			{
				UPrime = localU->getParent();
				betaPrime = localU->getLinkLabel() + 1;
				V = NodeHop(this->root, betaPrime, localU->getDepth() - 1, &cp[i], color);

				localU->setLink(V);
				
				pathToFind = &cp[i] + (this->getLenBetaPrime(V, UPrime));

				temp = findPath(V, pathToFind, color);
				localU = temp->getParent();
			}
		}
		if (temp->getID() == 43)
		{
			//the node with no link just created
		}

	}
	return NULL;
}


SuffixNode* SuffixTree::NodeHop(SuffixNode* t, char* path, int pathLen, char *s, int color) //Naive Find path alg
{

	if (pathLen == 0)
	{
		return t;
	}
	vector<SuffixNode*>* tChildren = t->getChildrenPointers();
	vector<SuffixNode*>::iterator it;
	char* temp, *cp = path;
	int VDepth = t->getDepth();
	it = tChildren->begin();
	while (it != tChildren->end())
	{
		temp = (*it)->getLinkLabel();

		if (*temp == *cp) // first character match
		{
			VDepth = (*it)->getDepth();
			if (VDepth > pathLen) //
			{
				
				int newDepth = VDepth - pathLen;
				char* newStart = ((*it)->getLinkLabel()) + pathLen;
				char* newPathStart = s + pathLen;
				int newPathDepth = strlen(newPathStart);
				
				SuffixNode* newInternalNode = new SuffixNode((*it)->getParent(), numNodes + 1, VDepth - newDepth, 0, (*it)->getLinkLabel(), NULL); //create new internal node between t and child *it
				this->numNodes += 1;
				newInternalNode->setColor(color);
				SuffixNode* newLeafNode = new SuffixNode(newInternalNode, this->numNodes + 1, newPathDepth, this->numLeafs + 1, newPathStart, NULL);
				this->numNodes += 1;
				this->numLeafs += 1;
				newLeafNode->setColor(color);

				(*it)->setString(newStart, newDepth); //change the start and lenght of *it for after the new internal node
				vector<SuffixNode*>* internalChildList = newInternalNode->getChildrenPointers(); //get the new internal node's children vector
				(*it)->setParent(newInternalNode); 

				if (*newPathStart == '$' || *newPathStart < *newStart)
				{
					internalChildList->push_back(newLeafNode);
					internalChildList->push_back(*it);
				}
				else
				{
					internalChildList->push_back(*it);
					internalChildList->push_back(newLeafNode);
				}

				tChildren->erase(it); // remove *it from t's child vector
				
				//Insert new internal node into t's child vector
				it = tChildren->begin();
				while (it != tChildren->end())
				{
					temp = (*it)->getLinkLabel();
					if (*temp == '$' || *cp < *temp) // insert in middle
					{
						tChildren->insert(it, newInternalNode);
						return newInternalNode;
					}
					else
					{
						it++;
					}
				}

				tChildren->push_back(newInternalNode);
				return newInternalNode;
				
			}
			else if(VDepth == pathLen)
			{
				return *it; // this is V

			}
			else {
				cp = cp + VDepth;
				pathLen = pathLen - VDepth;
				s = s + VDepth;
				return NodeHop((*it), cp, pathLen, s, color);
			}

		}
		it++;
	}

	return *it;
}

void SuffixTree::DFS()
{
	ofstream output;
	output.open("DFS.txt");
	int maxRepeatLength = 0, maxRepeatStart = 0;
	DFSHelper(this->root, &maxRepeatLength, &maxRepeatStart, &output);
	cout << endl;
	cout << "Max Repeat Length: " << maxRepeatLength << endl;
	cout << "Max Repeat: ";
	for (int i = 0; i < maxRepeatLength; i++)
	{
		cout << this->strFrontPtr[maxRepeatStart + i];
	}
	cout << endl;
	return;
}

void SuffixTree::DFSHelper(SuffixNode* t, int *maxRepeatLength, int *maxRepeatStart, ofstream *output)
{
	char character;
	int temp = 0;
	SuffixNode* tempNode;
	vector<SuffixNode*>* children = t->getChildrenPointers();
	int id = 0;
	if (children->size() == 0) // at the bottom of the tree
	{
		id = t->getLeafIndex() - 1;
		character = *t->getLinkLabel();

		*output << character << endl;;
		
		return;
	}
	vector<SuffixNode*>::iterator it = children->begin();
	while (it != children->end()) // internal node
	{
		DFSHelper(*it, maxRepeatLength, maxRepeatStart, output);;

		if ((*it)->getChildrenPointers()->size() != 0)
		{
			temp = this->getLenAlpha(*it);
			if (temp > *maxRepeatLength)
			{
				*maxRepeatLength = temp;
				tempNode = *it;
				while (tempNode->getParent() != this->root)
				{
					tempNode = tempNode->getParent();
				}
				*maxRepeatStart = (int)tempNode->getLinkLabel() - (int)this->strFrontPtr;
			}
			character = *(*it)->getLinkLabel();
			*output << character << endl;
		}

		
		it++;
	}
}

void SuffixTree::BWT() 
{
	ofstream out;
	out.open("Output.txt");
	out << "Original String Length: " << strlen(this->strFrontPtr) << endl;
	out << "Total Number of Nodes: " << this->numNodes << " Number of Leafs: " << this->numLeafs << " Number of internal nodes: " << (this->numNodes - this->numLeafs) << endl;
	ofstream outfile;
	outfile.open("BWT.txt");

	BWTHelper(this->root, &outfile);
}

void SuffixTree::BWTHelper(SuffixNode* t, ofstream *outfile)
{
	char character;
	vector<SuffixNode*>* children = t->getChildrenPointers();
	int id = 0;
	if (children->size() == 0) // at the bottom of the tree
	{
		id = t->getLeafIndex() - 1;
		if (id != 0)
		{
			character = this->strFrontPtr[id-1];
		}
		else
		{
			character = '$';

		}
		*outfile << character << endl;
		
		return;
	}
	vector<SuffixNode*>::iterator it = children->begin();
	while (it != children->end())
	{
		BWTHelper(*it, outfile);
		it++;
	}
}

int SuffixTree::getLenAlpha(SuffixNode* U)
{
	SuffixNode* temp = U;
	int runningCount = 0;
	while (temp != this->root)
	{
		runningCount += temp->getDepth();
		temp = temp->getParent();
	}
	return runningCount;
}

int SuffixTree::getLenBetaPrime(SuffixNode* V, SuffixNode* Uprime)
{
	SuffixNode* temp = V;
	int runningCount = 0;
	while (temp != Uprime && temp != this->root)
	{
		runningCount += temp->getDepth();
		temp = temp->getParent();
	}
	return runningCount;
}

void SuffixTree::colorNodes(SuffixNode *U)
{
	int childrenColor = 0;
	vector<SuffixNode*>* children = U->getChildrenPointers();
	for (int i = 0; i < children->size(); i++)// internal node
	{
		colorNodes(children->at(i));
		
	}
	if (!children->empty())
	{
		childrenColor = (children->at(0))->getColor();
		for (int i = 0; i < children->size(); i++)
		{
			if ((children->at(i)->getColor()) == childrenColor)
			{
				continue;
			}
			else
			{
				childrenColor = 3;
				break;
			}
		}
		U->setColor(childrenColor);
	}
}

vector<char> SuffixTree::findFingerprint(SuffixNode* U, int color)
{
	vector<char> fingerprint, holder;
	//iterate down until we find a child with a non 3 color then copy up to root and then the first char of that child
	vector<SuffixNode*>* children = U->getChildrenPointers();
	vector<SuffixNode*>* children2;
	SuffixNode* ptemp;
	if (children->empty())
	{
		return fingerprint;
	}
	vector<SuffixNode*>::iterator it = children->begin();
	vector<char>::iterator it2;
	for (int i = 0; i < children->size(); i++)
	{
		if (children->at(i)->getColor() == 3)
		{
			continue;
		}
		else if (*children->at(i)->getLinkLabel() == '$')
		{
			continue;
		}
		else
		{
			fingerprint.push_back(*children->at(i)->getLinkLabel());
			ptemp = children->at(i)->getParent();
			while (ptemp != this->root)
			{
				holder = ptemp->getParentEdgeLabel();
				holder.insert(holder.end(), fingerprint.begin(), fingerprint.end());
				fingerprint = holder;
				ptemp = ptemp->getParent();
			}
			return fingerprint;
		}
	}
	it = children->begin();
	for (int i = 0; i < children->size(); i++)
	{
		fingerprint = findFingerprint(children->at(i), color);
		if (!fingerprint.empty())
		{
			return fingerprint;
		}
	}
}

//vector<char> SuffixTree::findFingerprint(SuffixNode* U, int color)
//{
//	vector<char> fingerprint, holder;
//	SuffixNode* ptemp = U;
//	SuffixNode *retrace;
//	vector<SuffixNode*>* children, *children2;
//	while (ptemp != NULL)
//	{
//		children = ptemp->getChildrenPointers();
//		for (int i = 0; i < children->size(); i++)
//		{
//			if (children->at(i)->getColor() != 3)
//			{
//				children2 = children->at(i)->getChildrenPointers();
//				holder = children2->at(0)->getParentEdgeLabel();
//				fingerprint.push_back(holder[0]);
//				retrace = children->at(i);
//				while (retrace != root)
//				{
//					holder = retrace->getParentEdgeLabel();
//					holder.insert(holder.end(), fingerprint.begin(), fingerprint.end());
//					fingerprint = holder;
//				}
//				return fingerprint;
//			}
//		}
//	}
//}