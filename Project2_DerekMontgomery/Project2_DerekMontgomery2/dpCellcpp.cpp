#include "dpCell.h"

DpCell::DpCell() {
	this->score = 0;
	this->deletion = 0;
	this->insertion = 0;
	this->substituion = 0;
}

int DpCell::getScore() {
	return this->score;
}

int DpCell::getInsertion() {
	return this->insertion;
}

int DpCell::getDeletion() {
	return this->deletion;
}

int DpCell::getSubstitution() {
	return this->substituion;
}

void DpCell::setScore(int score) {
	this->score = score;
}

void DpCell::setInsertion(int insert) {
	this->insertion = insert;
}

void DpCell::setDeletion(int deletion) {
	this->deletion = deletion;
}

void DpCell::setSubstitution(int sub) {
	this->substituion = sub;
}