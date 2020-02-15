#include "project.h"

/*
    Derek Montgomery
    Computational Genomics Programming project 1

    Note: some issues unresolved at time of assignment submission due
    to deadline. This includes local alignment issue of str2 being changed
    after running global align.

*/
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

    char *s1Temp = (char *)malloc(strlen(str1) * sizeof(char));
    char *s2Temp = (char *)malloc(strlen(str2) * sizeof(char));
    strcpy(s1Temp, str1);
    strcpy(s2Temp, str2);
    printf("Scores: \t match: %d, mismatch: %d, h: %d, g: %d \n", match, mismatch, gapOpen, gapCont);
    printf("Global\n");
    globalAlign(s1Temp, s2Temp);
    fseek(in,0,SEEK_SET);
    fileProcess(in,res);
    printf("Local\n");
    strcpy(s1Temp, str1);
    strcpy(s2Temp, str2);
    printf("%s\n", s2Temp);
    localAlign(s1Temp, s2Temp);
    printf("End\n");
    return 0;

}

