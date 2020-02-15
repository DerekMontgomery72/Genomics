#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int match, mismatch, gapOpen, gapCont;
FILE *infile;
char s1Name[256],s2Name[256];
char *str1, *str2;

typedef struct dpcell{
    int score;
    int insertion;
    int deletion;
    int substitution;
}DPCELL;

int globalAlign(char *string1, char *string2);
int scoreChar(char ai, char bj);
int Max(int s1, int s2, int s3);
int maxDeletion(DPCELL** table, int i, int j);
int maxInsertion(DPCELL **table, int i, int j);
int maxSubstitution(DPCELL **table, int i, int j, char ai, char bj);
char * walkBack(DPCELL **table, int endI, int endJ, char *string1, char *string2);
char *insertGap(char *str, int index, int gapLen);
int MaxLocal(int s1, int s2, int s3);
int localAlign(char *string1, char *string2);
int fileProcess(FILE *fp, size_t size);





