#include "GlobalAlign.h"

int Align::globalAlign()
{
    int rows = s1.length(), columns = s2.length(), i, j;
    int tempInsert = 0, tempDel = 0, tempSub = 0;
    //Allocate Table
    // int len = sizeof(DPCELL *) *rows + sizeof(DPCELL) * columns * rows;
    DpCell** table = new DpCell *[rows];
    for (int i = 0; i < rows; ++i) {
        table[i] = new DpCell[columns];
    }

    //Set Row pointers

    // for(i = 0; i < rows; i++)
    // {
    //     table[i] = (ptr + columns * i);
    // }

    //Initialize Table row 0 scores
    for (i = 0; i < rows; i++)
    {
        table[i][0].setScore(0);
        table[i][0].setInsertion(0);
        table[i][0].setDeletion(0);
        table[i][0].setSubstitution(0);

    }
    //Initialize Table Column 0 scores
    for (j = 0; j < columns - 1; j++)
    {
        table[0][j].setScore(0);
        table[0][j].setInsertion(0);
        table[0][j].setDeletion(0);
        table[0][j].setSubstitution(0);
    }

    for (i = 1; i < rows; i++)
    {
        for (j = 1; j < columns; j++) {
            tempInsert = maxInsertion(table, i, j);
            tempSub = maxSubstitution(table, i, j, s1[i - 1], s2[j - 1]);
            tempDel = maxDeletion(table, i, j);
            table[i][j].setInsertion(tempInsert);
            table[i][j].setDeletion(tempDel);
            table[i][j].setSubstitution(tempSub);
            table[i][j].setScore(Max(tempInsert, tempDel, tempSub));
        }
    }

    string align = walkBack(table, (i - 1), (j - 1));

    for (int i = 0; i < rows; ++i) {
        delete[] table[i];
    }
    delete[] table;
}

int Align::maxInsertion(DpCell** table, int i, int j)
{
    int insertion, deletion, substitution;
    if (i == 0 && (j - 1) == 0) {
        return 0;
    }
    if ((j - 1) == 0) {
        //insertion is negative infinite
        //substitution is negative infinite
        return gapOpen + (i * gapCont);
    }
    if ((i - 1) == 0) {
        //substituion is negative infinite
        //deletion is negative infinite
        return (gapOpen + (j * gapCont));
    }
    else {
        insertion = table[i][j - 1].getInsertion() + gapCont;
        substitution = table[i][j - 1].getSubstitution() + gapOpen + gapCont;
        deletion = table[i][j - 1].getDeletion() + gapOpen + gapCont;
        return Max(insertion, substitution, deletion);
    }
}

int Align::maxDeletion(DpCell** table, int i, int j)
{
    int insertion, deletion, substitution;
    if ((i - 1) == 0 && j == 0) {
        return 0;
    }
    if (j == 0) {
        //insertion is negative infinite
        //substitution is negative infinite
        return gapOpen + (i * gapCont);
    }
    if ((i - 1) == 0) {
        //substituion is negative infinite
        //deletion is negative infinite
        return (gapOpen + (j * gapCont));
    }

    deletion = table[i - 1][j].getDeletion(); +gapCont;
    substitution = table[i - 1][j].getSubstitution() + gapOpen + gapCont;
    insertion = table[i - 1][j].getInsertion() + gapCont + gapOpen;

    return Max(deletion, substitution, insertion);
}

int Align::maxSubstitution(DpCell** table, int i, int j, char ai, char bj) {

    int insertion, deletion, substitution;
    int matchScore = scoreChar(ai, bj);

    if ((i - 1) == 0 && (j - 1) == 0) {
        return matchScore;
    }
    if ((j - 1) == 0) {
        //insertion is negative infinite
        //substitution is negative infinite
        return (gapOpen + (i * gapCont)) + matchScore;
    }
    if ((i - 1) == 0) {
        //substituion is negative infinite
        //deletion is negative infinite
        return (gapOpen + (j * gapCont)) + matchScore;
    }

    substitution = table[i - 1][j - 1].getSubstitution();
    deletion = table[i - 1][j - 1].getDeletion();
    insertion = table[i - 1][j - 1].getInsertion();
    int max = Max(insertion, deletion, substitution);
    return (max + matchScore);

}

int Align::scoreChar(char ai, char bj) {
    if (ai == bj) {
        return match;
    }
    else {
        return mismatch;
    }
}

int Max(int s1, int s2, int s3) {
    int max = s1;
    if (max < s2)
    {
        max = s2;
    }
    if (max < s3)
    {
        max = s3;
    }
    return max;
}
int MaxLocal(int s1, int s2, int s3)
{
    int max = 0;
    if (max < s1) {
        max = s1;
    }
    if (max < s2) {
        max = s2;
    }
    if (max < s3) {
        max = s3;
    }
    return max;
}

string Align::walkBack(DpCell** table, int endI, int endJ)
{
    //Maximum Length of Alignment = I*J
    int str1Len = s1.length(), str2Len = s2.length();

    char temp = '-';
    char* cp = &temp;
    int i, j, bottomGapLen = 0, topGapLen = 0;
    int len = (endI + endJ) * sizeof(char);
    int matches = 0, mismatches = 0, del = 0, insert = 0, gaps = 0, gapsStart = 0;
    int OptScore = table[endI][endJ].getScore();
    string alignmentTop = s1;
    string alignmentBottom = s2;

    i = endI;
    j = endJ;
    while (i > 0 && j > 0 && OptScore != 0)
    {
        if (i == 0 || j == 0) {
            if (i == 0) //At the edge of the columns -- can only do deletion until end
            {
                if (table[i][j - 1].getScore() == (OptScore - gapCont)) //check Deletion Continuing gap
                {
                    gaps = gaps + 1;
                    bottomGapLen = bottomGapLen + 1;
                    OptScore = OptScore - gapCont;
                    //Insert gap into bottom string
                }
                else if (table[i][j - 1].getScore() == (OptScore - (gapCont + gapOpen)))
                {
                    gaps = gaps + 1;
                    bottomGapLen = bottomGapLen + 1;
                    gapsStart = gapsStart + 1;

                }
                else {
                    //Error
                    cout << "Error Scores on table don't match possible movements" << endl;
                    break;
                }
                alignmentBottom = insertGap(alignmentBottom, j, bottomGapLen);
                bottomGapLen = 0;
                j = j - 1;
            }
            if (j == 0)//At top row -- can only to insertion until end
            {
                if (table[i - 1][j].getScore() == (OptScore - gapCont)) //check insertion Continuing gap
                {
                    gaps = gaps + 1;
                    topGapLen = topGapLen + 1;
                    OptScore = OptScore - gapCont;
                    //Insert gap into bottom string
                }
                else if (table[i - 1][j].getScore() == (OptScore - (gapCont + gapOpen)))
                {
                    gaps = gaps + 1;
                    gapsStart = gapsStart + 1;
                    topGapLen = topGapLen + 1;

                }
                else {
                    //Error
                    cout << "Error Scores not matching possible values" << endl;
                    break;
                }
                alignmentTop = insertGap(alignmentTop, i, topGapLen);
                topGapLen = 0;
                i = i - 1;
            }
        }
        else
        {
            if (OptScore == table[i][j].getInsertion()) // was insertion
            {
                if ((OptScore - gapCont) == table[i][j - 1].getInsertion())
                {
                    gaps = gaps + 1;
                    topGapLen++;
                    OptScore = table[i][j - 1].getScore();
                    i = i - 1;
                    continue;
                }
                else if ((OptScore - (gapOpen + gapCont) == table[i][j - 1].getSubstitution()) || ((OptScore - (gapOpen + gapCont) == table[i][j - 1].getDeletion())))
                {
                    gapsStart = gapsStart + 1;
                    gaps = gaps + 1;
                    topGapLen++;
                    OptScore = table[i][j - 1].getScore();
                    alignmentTop = insertGap(alignmentTop, i, topGapLen);
                    topGapLen = 0;
                    j = j - 1;
                    continue;
                }

            }
            if (OptScore == table[i][j].getDeletion()) //deletion
            {
                if ((OptScore - gapCont) == table[i - 1][j].getDeletion())
                {
                    gaps = gaps + 1;
                    bottomGapLen++;
                    OptScore = table[i - 1][j].getScore;
                    i = i - 1;
                    continue;
                }
                else if ((OptScore - (gapOpen + gapCont) == table[i - 1][j].getSubstitution()) || ((OptScore - (gapOpen + gapCont)) == table[i - 1][j].getInsertion()))
                {
                    gapsStart = gapsStart + 1;
                    gaps = gaps + 1;
                    bottomGapLen++;
                    OptScore = table[i - 1][j].getScore();
                    i = i - 1;
                    alignmentBottom = insertGap(alignmentBottom, i, bottomGapLen);
                    bottomGapLen = 0;
                    continue;

                }

            }
            if (OptScore == table[i - 1][j - 1].getScore() + match)
            {
                OptScore = table[i - 1][j - 1].getScore();
                i = i - 1;
                j = j - 1;
                matches = matches + 1;
                continue;
            }
            if (OptScore == table[i - 1][j - 1].getScore() + mismatch) {
                OptScore = table[i - 1][j - 1].getScore();
                i = i - 1;
                j = j - 1;
                mismatches = mismatches + 1;
                continue;
            }


        }
    } //end WHile loop
    int endLen = alignmentTop.length();

    double Identities = 0.0, gapRatio = 0.0;
    Identities = ((double)matches / (double)endLen) * 100;
    gapRatio = ((double)gaps / (double)endLen) * 100;
    printf("Optimal Score: %d, matches: %d, mismatches: %d, gaps: %d, gapStarts: %d\n", table[endI][endJ].getScore(), matches, mismatches, gaps, gapsStart);
    printf("Match Ratio:(%d/%d) %f%%, Gap Ratio (%d/%d) %f%%\n", gaps, endLen, Identities, gaps, endLen, gapRatio);

    printf("%s\n", alignmentTop);
    printf("%s\n", alignmentBottom);
    return alignmentBottom;


}

string insertGap(string str, int index, int gapLen) // string has memory available allocated in outer function
{
    string dest;
    char insert = '-'; // character to be inserted
    int sourceLen = str.length(), i;
    dest = str.substr(index - 1);
 
    string buf(str, 0, index);

    for (i = 0; i < gapLen; i++) {
        buf = buf + "-";
    }
    buf[(index + i)] = '\0';
    buf = buf + dest;

    str = buf;
    return str;

}

