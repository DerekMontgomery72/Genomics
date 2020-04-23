#pragma once
#include <iostream>
#include <string>


class DpCell {


public:
	DpCell();


	int getScore();
	int getInsertion();
	int getDeletion();
	int getSubstitution();

	void setScore(int score);
	void setInsertion(int insert);
	void setDeletion(int deletion);
	void setSubstitution(int sub);

private:
	int score;
	int insertion;
	int deletion;
	int substituion;

};