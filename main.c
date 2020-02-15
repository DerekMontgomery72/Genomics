#include "project.h"


char *TestString1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
char *TestString2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";

int main(int argc, char **argv)
{
    match = 1;
    mismatch = -2;
    gapCont = -2;
    gapOpen = -5;

    printf("Run Global Align\n");
    globalAlign(TestString1, TestString2);
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
        
    }
    //Initialize Table Column 0 scores
    for(j = 0; j < columns-1; j++)
    {
        table[0][j].score = 0;
        table[0][j].insertion = 0;
        table[0][j].deletion = 0;
        table[0][j].substitution = 0;
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
        }
        printf("\n");
    }

    printf("Maximum Global Alignment Score: %d\n", table[i -1][j -1].score);

    char *aling = walkBack(table,(i-1),(j-1),string1,string2);
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
     insertion = table[i][j-1].insertion + gapCont;
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

char * walkBack(DPCELL **table, int endI, int endJ, char *string1, char *string2)
{
    //Maximum Length of Alignment = I*J
    char temp = '-';
    char *cp = &temp;
    int i, j;
    int len = (endI + endJ) * sizeof(char);
    int matches = 0, mismatches = 0, del = 0, insert = 0, gaps = 0, gapsStart = 0;
    int OptScore = table[endI][endJ].score;
    char *alignmentTop = (char *)malloc(len);
    char *alignmentBottom = (char*)malloc(len);
    strcpy(alignmentTop,string1);
    strcpy(alignmentBottom,string2);

    i = endI;
    j = endJ;
    while(i > 0 && j > 0)
    {
        if( i == 0 || j == 0){
            if(i == 0) //At the edge of the columns -- can only do deletion until end
            {
                if(table[i][j-1].score == (OptScore - gapCont)) //check Deletion Continuing gap
                {
                    gaps = gaps + 1;
                    OptScore = OptScore - gapCont;
                    //Insert gap into bottom string
                }
                else if(table[i][j-1].score == (OptScore - (gapCont + gapOpen)))
                {
                    gaps = gaps +1;
                    gapsStart = gapsStart + 1;

                }
                else{
                    //Error
                    printf("Error Scores on table don't match possible movements\n"); 
                    break;
                }
                alignmentBottom = insertGap(alignmentBottom,j);
                j = j-1;
            }
            if(j == 0)//At top row -- can only to insertion until end
            {
                if(table[i-1][j].score == (OptScore - gapCont)) //check insertion Continuing gap
                {
                    gaps = gaps + 1;
                    OptScore = OptScore - gapCont;
                    //Insert gap into bottom string
                }
                else if(table[i-1][j].score == (OptScore - (gapCont + gapOpen)))
                {
                    gaps = gaps +1;
                    gapsStart = gapsStart + 1;

                }
                else{
                    //Error
                    printf("Error Scores not matching possible values\n");
                    break;
                }
                alignmentTop = insertGap(alignmentTop, i);
                i = i-1;
            }
        }
        else
        {
            if(OptScore == table[i][j].insertion) // was insertion
            {
                if((OptScore - gapCont) == table[i][j-1].insertion)
                {
                    gaps = gaps + 1;
                    OptScore = table[i][j-1].score;
                    alignmentTop = insertGap(alignmentTop, i);
                    i = i-1;
                    continue;
                }
                else if((OptScore - (gapOpen + gapCont) == table[i][j-1].substitution) || ((OptScore -(gapOpen + gapCont) == table[i][j-1].deletion)))
                {
                    gapsStart = gapsStart + 1;
                    gaps = gaps + 1;
                    OptScore = table[i][j-1].score;
                    alignmentTop = insertGap(alignmentTop, i);
                    j = j-1;
                    continue;
                }

            }
            if(OptScore == table[i][j].deletion) //deletion
            {
                if((OptScore - gapCont) == table[i-1][j].deletion)
                {
                    gaps = gaps + 1;
                    OptScore = table[i-1][j].score;
                    i = i-1;
                    alignmentBottom = insertGap(alignmentBottom, i);
                    continue;
                }
                else if((OptScore - (gapOpen + gapCont) == table[i-1][j].substitution) || ((OptScore -(gapOpen + gapCont)) == table[i -1][j].insertion))
                {
                    gapsStart = gapsStart + 1;
                    gaps = gaps + 1;
                    OptScore = table[i-1][j].score;
                    i = i-1;
                    alignmentBottom = insertGap(alignmentBottom, i);
                    continue;

                }
               
            }
            if(OptScore == table[i-1][j-1].score + match)
            {
                OptScore = table[i-1][j-1].score;
                i = i-1;
                j = j-1;
                matches = matches + 1;
                continue;
            }
            if(OptScore == table[i-1][j-1].score + mismatch){
                OptScore = table[i-1][j-1].score;
                i = i-1;
                j = j-1;
                mismatches = mismatches + 1;
                continue;
            }
            
        
        }
        
        

    } //end WHile loop

    printf("Score: %d, matches: %d, mismatches: %d, gaps: %d, gapStarts: %d\n ", table[endI][endJ].score,matches, mismatches,gaps,gapsStart);

    printf("%s\n",alignmentTop);
    printf("%s\n",alignmentBottom);
    return alignmentBottom;


}

char *insertGap(char *str, int index) // string has memory available allocated in outer function
{
    char *strEnd = str + index;
    char insert = "-";
    char *strTemp = (char*)malloc(sizeof(char) * (strlen(str) - index)); //Create string spacce for back portion of string;
    char *buf = (char *)malloc(sizeof(char) *strlen(str));
    strncpy(buf,str,index);
    index += strlen(buf);
    strcpy(buf + index, insert);
    index += strlen(insert);
    strcpy(buf+index, str+index);
    return buf;

}
