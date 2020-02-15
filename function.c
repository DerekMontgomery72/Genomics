#include "project.h"



int localAlign(char *string1, char *string2)
{
    int rows = strlen(string1), columns =strlen(string2), i, j;
    int tempInsert = 0, tempDel = 0, tempSub = 0;
    //Allocate Table
    int len = sizeof(DPCELL *) *rows + sizeof(DPCELL) * columns * rows;
    DPCELL **table = (DPCELL**)malloc(len);
    DPCELL *ptr = (DPCELL *)(table + rows);
    DPCELL *maxCell = NULL;
    int maxI, maxJ;
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
    maxCell = &table[0][0];

    for(i = 1; i < rows; i++)
    {
        for(j = 1; j < columns; j++){
            tempInsert = maxInsertion(table, i, j);
            tempSub = maxSubstitution(table, i, j, string1[i-1], string2[j-1]);
            tempDel = maxDeletion(table, i, j);
            table[i][j].insertion = tempInsert;
            table[i][j].deletion = tempDel;
            table[i][j].substitution = tempSub;
            table[i][j].score = MaxLocal(tempInsert, tempDel, tempSub);
            if(table[i][j].score > maxCell->score)
            {
                maxCell = &table[i][j];
                maxI = i;
                maxJ = j;
            }
        }
    }
    printf("MaxScore: %d\n", maxCell->score);
    char *align = walkBack(table,(maxI),(maxJ),string1,string2);
    free(table);

}

int globalAlign(char *string1, char *string2)
{
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
        }
    }

    char *align = walkBack(table,(i-1),(j-1),string1,string2);
    free(table);
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
int MaxLocal(int s1, int s2, int s3)
{
    int max = 0;
    if(max < s1){
        max = s1;
    }
    if(max < s2){
        max = s2;
    }
    if(max < s3){
        max = s3;
    }
    return max;
}

char * walkBack(DPCELL **table, int endI, int endJ, char *string1, char *string2)
{
    //Maximum Length of Alignment = I*J
    int str1Len = strlen(string1), str2Len = strlen(string2);
    
    char temp = '-';
    char *cp = &temp;
    int i, j, bottomGapLen = 0, topGapLen = 0;
    int len = (endI + endJ) * sizeof(char);
    int matches = 0, mismatches = 0, del = 0, insert = 0, gaps = 0, gapsStart = 0;
    int OptScore = table[endI][endJ].score;
    char *alignmentTop = (char *)malloc(len);
    char *alignmentBottom = (char*)malloc(len);
    strcpy(alignmentTop,string1);
    strcpy(alignmentBottom,string2);
    printf("Sequence 1 = %s, length %d\n", s1Name, str1Len);
    printf("Sequence 2: %s, length %d\n",s2Name, str2Len);
    i = endI;
    j = endJ;
    while(i > 0 && j > 0 && OptScore != 0)
    {
        if( i == 0 || j == 0){
            if(i == 0) //At the edge of the columns -- can only do deletion until end
            {
                if(table[i][j-1].score == (OptScore - gapCont)) //check Deletion Continuing gap
                {
                    gaps = gaps + 1;
                    bottomGapLen = bottomGapLen + 1;
                    OptScore = OptScore - gapCont;
                    //Insert gap into bottom string
                }
                else if(table[i][j-1].score == (OptScore - (gapCont + gapOpen)))
                {
                    gaps = gaps +1;
                    bottomGapLen = bottomGapLen + 1;
                    gapsStart = gapsStart + 1;

                }
                else{
                    //Error
                    printf("Error Scores on table don't match possible movements\n"); 
                    break;
                }
                alignmentBottom = insertGap(alignmentBottom,j, bottomGapLen);
                bottomGapLen = 0;
                j = j-1;
            }
            if(j == 0)//At top row -- can only to insertion until end
            {
                if(table[i-1][j].score == (OptScore - gapCont)) //check insertion Continuing gap
                {
                    gaps = gaps + 1;
                    topGapLen = topGapLen + 1;
                    OptScore = OptScore - gapCont;
                    //Insert gap into bottom string
                }
                else if(table[i-1][j].score == (OptScore - (gapCont + gapOpen)))
                {
                    gaps = gaps +1;
                    gapsStart = gapsStart + 1;
                    topGapLen = topGapLen + 1;

                }
                else{
                    //Error
                    printf("Error Scores not matching possible values\n");
                    break;
                }
                alignmentTop = insertGap(alignmentTop, i, topGapLen);
                topGapLen = 0;
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
                    topGapLen++;
                    OptScore = table[i][j-1].score;                    
                    i = i-1;
                    continue;
                }
                else if((OptScore - (gapOpen + gapCont) == table[i][j-1].substitution) || ((OptScore -(gapOpen + gapCont) == table[i][j-1].deletion)))
                {
                    gapsStart = gapsStart + 1;
                    gaps = gaps + 1;
                    topGapLen++;
                    OptScore = table[i][j-1].score;
                    alignmentTop = insertGap(alignmentTop, i, topGapLen);
                    topGapLen = 0;
                    j = j-1;
                    continue;
                }

            }
            if(OptScore == table[i][j].deletion) //deletion
            {
                if((OptScore - gapCont) == table[i-1][j].deletion)
                {
                    gaps = gaps + 1;
                    bottomGapLen++;
                    OptScore = table[i-1][j].score;
                    i = i-1;
                    continue;
                }
                else if((OptScore - (gapOpen + gapCont) == table[i-1][j].substitution) || ((OptScore -(gapOpen + gapCont)) == table[i -1][j].insertion))
                {
                    gapsStart = gapsStart + 1;
                    gaps = gaps + 1;
                    bottomGapLen++;
                    OptScore = table[i-1][j].score;
                    i = i-1;
                    alignmentBottom = insertGap(alignmentBottom, i, bottomGapLen);
                    bottomGapLen = 0;
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
    int endLen = strlen(alignmentTop);

    double Identities = 0.0, gapRatio = 0.0;
    Identities = ((double)matches / (double)endLen) * 100;
    gapRatio = ((double)gaps / (double)endLen) * 100;
    printf("Optimal Score: %d, matches: %d, mismatches: %d, gaps: %d, gapStarts: %d\n", table[endI][endJ].score,matches, mismatches,gaps,gapsStart);
    printf("Match Ratio:(%d/%d) %f%%, Gap Ratio (%d/%d) %f%%\n",gaps, endLen,Identities,gaps, endLen, gapRatio);

    printf("%s\n",alignmentTop);
    printf("%s\n",alignmentBottom);
    return alignmentBottom;


}

char *insertGap(char *str, int index, int gapLen) // string has memory available allocated in outer function
{
    char insert = '-', *dest; // character to be inserted
    int sourceLen = strlen(str), i;
    char *strEndBuf = (char *)malloc(sizeof(char) * (sourceLen));// buffer to hold end of string
    dest = str + index - 1;
    char *buf = (char *)malloc(sizeof(char) *strlen(str)); // buffer to hold built string
    strcpy(strEndBuf,dest);
    strncpy(buf,str,index); // move first part of string to buf
    for(i = 0; i < gapLen; i++){
        buf[index + i] = '-';
    }
    buf[(index + i)] = '\0';
    strcat(buf, strEndBuf);


    strcpy(str, buf);
    return str;

}
 
int fileProcess(FILE *fp, int size)
{
    char st1[1024], st2[1024], *file, buf[1024], *s1 = st1,*s2 = st2 ;
    char *cp, *temp;
    int ch, s1Size = 1024, s2Size = 1024, s1Read = 0;

    strcpy(s1,""); // set Empty string for s1 and s2
    strcpy(s2,""); 

    size_t len = 0;
    file = realloc(NULL, sizeof(char) *size); //Allocate enough memory to hold flie
    while(fgets(buf, 1024, fp) != NULL){
        if(buf[0] == '>'){
            buf[strlen(buf) -2] = '\0';
            //Name
            cp = buf + 1;
            if(strcmp(s1Name,"") == 0){
                strcpy(s1Name,cp);
            }
            else{
                strcpy(s2Name, cp);
            }
        }
        else if(strcmp(buf,"\n") == 0)
        {
            s1Read = 1; //Set s1 as already been read
            continue;
        }
        else
        {
            buf[strlen(buf) -2] = '\0';
            if(s1Read == 0){
                if(strlen(s1) + strlen(buf) > s1Size)
                {
                    temp = realloc(s1, (strlen(s1) + strlen(buf))); //allocate more space for s1
                    s1Size = strlen(s1) + strlen(buf);
                    if(!temp){
                        free(s1);
                        s1 = NULL;
                        break;
                    }
                    s1 = temp;
                }
                strcat(s1,buf);
            }
            else{
                if(strlen(s2) + strlen(buf) > s2Size){
                    s2Size = strlen(s2) + strlen(buf);
                    temp = realloc(s2, (strlen(s2) + strlen(buf)));
                    if(!temp){
                        free(s2);
                        s1 = NULL;
                        break;
                    }
                    s2 = temp;
                }
                strcat(s2,buf);
            }
        }

    }
    
    str1 = s1;
    str2 = s2;
    printf("str1 %s\n", str1);
    printf("str2: %s\n", str2);
    return 0;
    
}