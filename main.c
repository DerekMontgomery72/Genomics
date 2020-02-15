#include "project.h"


char *TestString1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
char *TestString2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";

int main(int argc, char **argv)
{

    match = 1;
    mismatch = -2;
    gapCont = -2;
    gapOpen = -5;

    strcpy(s1Name, "");
    strcpy(s2Name, ""); // Default Names

    printf("FILE: %s\n", argv[1]);
    if(argc == 1){
        printf("No file Given\n");
        return 0;
    }

    FILE *in;
    
    in = fopen(argv[1], "r");
    if(in == NULL){
        printf("File not Found\n");
        return 0;
    }
    // Get file size
    fseek(in,0L,SEEK_END);
    int res = ftell(in);
    printf("Input file size in bytes %d\n", res);
    fseek(in,0,SEEK_SET);


    fileProcess(in, res);


    printf("Scores: \t match: %d, mismatch: %d, h: %d, g: %d \n", match, mismatch, gapOpen, gapCont);
    printf("Global\n");
    globalAlign(str1, str2);
    printf("Local\n");
    localAlign(str1, str2);
    printf("End\n");
    return 0;

}

