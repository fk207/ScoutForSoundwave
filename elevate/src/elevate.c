#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "elevate.h"

int main(int argc, char **argv){
    FILE *inputf;
    
    inputf = fopen(argv[1], "rb");
    if(inputf == NULL){
        printf("Failed to open file");
        return 1;
    }

    int peopol; 
    if(!(fscanf(inputf,"%d",&peopol) == 1)){}

    int lim;
    if(!(fscanf(inputf,"%d",&lim) == 1)){}
    
    //Getting the newline character(?)
    fgetc(inputf);
    
    //Allocating the array for the requested floors
    int *stops = (int *)malloc(peopol * sizeof(int));
    if(stops == NULL) printerror((char *)"Failed to allocate stops array");

    int i;

    //Reading the requested floor from the file
    for( i = 0; fscanf(inputf,"%d",&stops[i]) == 1 && i < peopol; i++){}


    //=================================================================
    //=========================Mode Selection==========================
    //=================================================================

    if(argc < 2)    printf("Please enter input file");  //No input file 
    else if(argc < 3)  dp(peopol,lim,stops,0);          //When there is no specified mode (default behaviour)
    else if(strcmp(argv[2],"--mode=recurse") == 0)          recursive(peopol,lim,stops);
    else if(strcmp(argv[2],"--mode=brute")== 0)             brute(peopol,lim,stops);
    else if(strcmp(argv[2],"--mode=memoize")== 0)           memoize(peopol,lim,stops);
    else if(strcmp(argv[2],"--mode=dp")== 0){        
        if(argc >= 4 && strcmp(argv[3],"--debug") == 0 )    dp(peopol,lim,stops,1);
        else dp(peopol,lim,stops,0);
    }
    else if(strcmp(argv[2],"--debug")== 0){
        if((argc >= 4 && strcmp(argv[3],"--mode=dp")== 0)|| argc == 3)  dp(peopol,lim,stops,1);
        else    printf("This option does not support debugging");
    }
    else        printf("Invalid option");
     

    //===================Cleanup======================
    free(stops);
    fclose(inputf);
    return 0;
}
