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
    //globalAlign(TestString1, TestString2);
    localAlign(TestString1, TestString2);

}

