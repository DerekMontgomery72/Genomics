#include "project.h"


char *TestString1 = "acatgctacacgtactccgataccccgtaaccgataacgatacacagacctcgtacgcttgctacaacgtactctataaccgagaacgattgacatgcctcgtacacatgctacacgtactccgatgaccccgt";
char *TestString2 = "acattctacgaacctctcgataaccccataaccgataacgattgacacctcgtacgctttctacaacttactctctcgataaccccataaccgataacgattgacacctcgtacacatggtacatacgtactctcgataccccgt";

int main(int argc, char **argv)
{
    match = 1;
    mismatch = -2;
    gapCont = -1;
    gapOpen = -5;

    printf("Run Global Align\n");
    globalAlign(TestString1, TestString1);
}

int globalAlign(char *string1, char *string2)
{
    char c;
    int rows = strlen(string1), columns =strlen(string2), i, j;
    int tempInsert = 0, tempDel = 0, tempSub = 0;
    //Allocate Table
    int len = sizeof(DPCELL *) *rows + sizeof(DPCELL) * columns * rows;
    DPCELL **table = (DPCELL**) malloc(len);
    DPCELL *ptr = (DPCELL *)(table + rows);
    //Set Row pointers
    for(i = 0; i < rows; i++)
    {
        table[i] = (ptr + columns * i);
    }

    //Initialize Table row 0 scores
    for(i = 0; i < rows; i++)
    {
        table[i][0].score = 0;
        table[i][0].insertion = 0;
        table[i][0].deletion = 0;
        table[i][0].substitution = 0;
        printf("[ ");
    }
    //Initialize Table Column 0 scores
    for(j = 0; j < columns; j++)
    {
        table[j][0].score = 0;
        table[j][0].insertion = 0;
        table[j][0].deletion = 0;
        table[j][0].substitution = 0;
    }

    for(i = 1; i < rows; i++)
    {
        for(j = 1; j < columns; j++){
            tempInsert = maxInsertion(table, i, j);
            tempSub = maxSubstitution(table, i, j, string1[i-1], string2[j-1]);
            tempDel = maxDeletion(table, i, j);
            table[i][j].insertion = tempInsert;
            table[i][j].deletion = tempDel;
            table[i][j].substitution = tempSub;
            table[i][j].score = Max(tempInsert, tempDel, tempSub);
            printf("[%d][%d]: sub: %d, del: %d, Insert: %d, Score: %d\n ",i,j, tempSub, tempDel, tempInsert, table[i][j].score);
            getc(c);
        }
        printf("\n");
    }

    printf("Maximum Global Alignment Score: %d\n", table[i -1][j -1].score);
}

int maxInsertion(DPCELL **table, int i, int j)
{
    int insertion, deletion, substitution;
    if( i == 0 && (j-1) == 0){
        return 0;
    }
    if((j-1) == 0){
        //insertion is negative infinite
        //substitution is negative infinite
        return gapOpen + (i*gapCont);
    }
    if((i -1) == 0){
        //substituion is negative infinite
        //deletion is negative infinite
        return (gapOpen + (j * gapCont));
    }
    else{
     insertion = table[i][j-1].insertion;
     substitution = table[i][j-1].substitution + gapOpen + gapCont;
     deletion = table[i][j-1].deletion + gapOpen + gapCont;
    return Max(insertion, substitution, deletion);
    }
     
}

int maxDeletion(DPCELL** table, int i, int j)
{
    int insertion, deletion, substitution;
    if( (i-1) == 0 && j == 0){
        return 0;
    }
    if(j == 0){
        //insertion is negative infinite
        //substitution is negative infinite
        return gapOpen + (i*gapCont);
    }
    if((i -1) == 0){
        //substituion is negative infinite
        //deletion is negative infinite
        return (gapOpen + (j * gapCont));
    }
    
    deletion = table[i -1][j].deletion + gapCont;
    substitution = table[i-1][j].substitution + gapOpen + gapCont;
    insertion = table[i-1][j].insertion + gapCont + gapOpen;

    return Max(deletion, substitution, insertion);
}

int maxSubstitution(DPCELL **table, int i, int j, char ai, char bj){

    int insertion, deletion, substitution;
    int matchScore = scoreChar(ai, bj);

    if( (i-1) == 0 && (j-1) == 0){
        return matchScore;
    }
    if((j-1) == 0){
        //insertion is negative infinite
        //substitution is negative infinite
        return (gapOpen + (i*gapCont)) + matchScore;
    }
    if((i -1) == 0){
        //substituion is negative infinite
        //deletion is negative infinite
        return (gapOpen + (j * gapCont)) + matchScore;
    }

    substitution = table[i-1][j-1].substitution;
    deletion = table[i-1][j-1].deletion;
    insertion = table[i-1][j-1].insertion;
    int max = Max(insertion, deletion, substitution);
    return (max + matchScore);

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