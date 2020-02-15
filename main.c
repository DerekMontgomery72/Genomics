#include "project.h"


char *TestString1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
char *TestString2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";

int main(int argc, char **argv)
{
    printf("FILE: %s\n", argv[1]);
    if(argc == 1){
        printf("No file Given\n");
        return;
    }

    FILE *in;
    in = fopen(argv[1], "r");
    if(in == NULL){
        printf("File not Found\n");
        return 0;
    }
    // Get file size
    fseek(in,0L,SEEK_END);
    long int res = ftell(in);
    printf("Input file size in bytes %ld\n", res);
    fclose(in);

    in = fopen(argv[1], "r");
    fileProcess(in, (size_t)res);

    strcpy(s1Name, "");
    strcpy(s2Name, ""); // Default Names

    match = 1;
    mismatch = -2;
    gapCont = -2;
    gapOpen = -5;
    printf("Scores: \t match: %d, mismatch: %d, h: %d, g: %d \n", match, mismatch, gapOpen, gapCont);
    printf("Global\n");
    globalAlign(TestString1, TestString2);
    printf("Local\n");
    localAlign(TestString1, TestString2);
    printf("End\n");
    return 0;

}

