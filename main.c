#include "project.h"


char *TestString1 = "acatgctacacgtactccgataccccgtaaccgataacgatacacagacctcgtacgcttgctacaacgtactctataaccgagaacgattgacatgcctcgtacacatgctacacgtactccgatgaccccgt";
char *TestString2 = "acattctacgaacctctcgataaccccataaccgataacgattgacacctcgtacgctttctacaacttactctctcgataaccccataaccgataacgattgacacctcgtacacatggtacatacgtactctcgataccccgt";

int main(int argc, char **argv)
{
    match = 1;
    mismatch = -2;
    gap = -1;
}

int globalAlign(char *string1, char *string2)
{
    int rows = stlen(string1), columns =strlen(string2), i, j;
    //Allocate Table
    int len = sizeof(int *) *rows + sizeof(int) * columns * rows;
    int **table = (int**) malloc(len);
    int *ptr = (int *)(table + rows);
    //Set Row pointers
    for(i = 0; i < rows; i++)
    {
        table[i] = (ptr + columns * i);
    }

    //Initialize Table row 0 scores
    for(i = 0; i < rows; i++)
    {
        table[i][0] = i*gap;
    }
    //Initialize Table Column 0 scores
    for(j = 0; j < columns; j++)
    {
        table[0][j] = j*gap;
    }

    for(i = 1; i < rows; i++)
    {
        for(j = 1; j < columns; j++){
            //Set current cell to Max of scores
            table[i][j] = Max((table[i-1][j-1] + scoreChar(string1[i-1],string2[j-1])),
                            (table[i-1][j] + gap), 
                            (table[i][j-1] + gap));
        }
    }

    printf("Maximum Global Alignment Score: %d", table[i][j]);




}

int scoreChar(char ai, char bj){
    if(ai == bj){
        return match;
    }
    else{
        return mismatch;
    }
}

int Max(int s1, int s2, int s3){
    int max = s1;
    if(max < s2)
    {
        max = s2;
    }
    if(max < s3)
    {
        max = s3;
    }
    return max;
}