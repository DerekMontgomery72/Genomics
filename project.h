#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int match, mismatch, gap;

int globalAlign(char *string1, char *string2);
int scoreChar(char ai, char bj);
int Max(int s1, int s2, int s3);
