#include <stdio.h>
#include <stdlib.h>
#include "elevate.h"

typedef struct minpairS{
    int val;
    int floor;
}minpair;


int Dfw(int a, int b, int * flrs, int n, int maxf);
void fillM( int stops, int * flrs, int people, minpair ** memoi, int maxf, int debug);
int * Dfindpath(int stops, minpair ** memoi, int maxf);

void dp(int people, int stops, int * reqfloors, int debug){ 


    //We are sorting the array to optimize the floors walked function, since this way we will be able to instantly skip all the passengers
    //with destinations larger than the intended range
    qsort(reqfloors,people,sizeof(int),compare);
    
    //Since the array is sorted, the maximum 
    int maxf = reqfloors[people-1];
    
    //Allocating the array where both the cost and the floors it was calculated are stored
    //One array is used to determine the minimum score and the other is used to calculate
    //the path the elevator takes

    minpair **M = (minpair **)malloc((stops + 1) * sizeof(minpair *));
    if(M == NULL) printerror((char *)"Failed to allocate M array");

    for(int i = 0; i <= stops; i++){
        M[i] = (minpair *)malloc((maxf + 1) * sizeof(minpair));
        if(M[i] == NULL) printerror((char *)"Failed to allocate M array"); 
        for(int j = 0; j <= maxf; j++){
            M[i][j].val = -1;
            M[i][j].floor = 0;
        }
    }

    //Calling the main calculation function to fill the array 
    fillM(stops,reqfloors,people,M,maxf,debug);

    //Calculating the path
    int *path = Dfindpath(stops,M,maxf);

    if(stops > 1){
        printf("Lift stops are:");
        for(int i = 1; i <= stops; i++)  if(path[i] != 0)   printf(" %d", path[i]);
    }
    else printf("No lift stops");
    printf("\nThe minimum cost is: %d\n",path[stops+1]);    


    //=====================Cleanup======================
    free(path);
    for(int i = 0; i<= stops; i++){
        free(M[i]);
    }
    free(M);

}



int * Dfindpath(int stops, minpair ** memoi, int maxf){
    int *path = (int *)malloc((stops + 2) * sizeof(int));
    if(path == NULL) printerror((char *)"Failed to allocate path array");
    
    
    int min = -1; //Stores the minimum total cost found
    int floor = -1; //Stores the floor with the minimum cost

    //Starting with calculating the last floor by scanning the array 
    for (int j = 1; j <= maxf; j++) {
        if (min == -1 || memoi[stops][j].val < min) {
            min = memoi[stops][j].val;
            floor = j;
        }
    }

    //Using backtracking to find the previous stops
    //We start from the index of the previous stop and follow the path dictated by the second array
    //Visualising the process, it basically goes to the last occurance of the curent coordinates of the
    //floor array, goes up a line and stores that value.

    //Better visualisation in the readme
    for (int s = stops; s > 0; s--) {
        path[s] = floor;
        floor = memoi[s][floor].floor; 
    }
    path[stops+1]=min;//Storing the cost in the last extra spot of the array. Not needed, just simplifies the process a little  bit
    return path;
}


void fillM( int stops, int * flrs, int people, minpair ** memoi, int maxf,int debug){

    int temp = Dfw(0, -1, flrs, people, maxf);
    for(int l = 0; l <= maxf; l++){
        memoi[0][l].val = temp;// The calculation is the same for every value of j, so we can set it like that instantly
        memoi[0][l].floor = 0; // Parent of row 0 is always 0
    }

    //Starting building the M table from the second row downwards since we already built the first one
    for(int i = 1; i <= stops;i++){
        for (int j = 0; j <= maxf; j++){

            //We start by assuming the first calculation is the minimum 
            int curmin = memoi[i-1][0].val - Dfw(0,-1,flrs,people,maxf) + Dfw(0,j,flrs,people,maxf) + Dfw(j,-1,flrs,people,maxf); 
            int fl = 0;

            //and we check all the next ones to find any lower numbers
            for(int k = 1; k <= maxf; k++){
                int cand = memoi[i-1][k].val - Dfw(k,-1,flrs,people,maxf) + Dfw(k,j,flrs,people,maxf) + Dfw(j,-1,flrs,people,maxf); 
                if(cand < curmin){
                    curmin = cand;
                    fl = k; 
                }
            }
            memoi[i][j].val = curmin;//In the end, we store both the minimum cost of said floor
            memoi[i][j].floor = fl;//As well as the floor it was found in
        }
    }
    
    //Printing the debug menu
    if(debug){    
        for(int i = 0; i <= stops;i++){
            printf("%d",memoi[i][0].val);
            for(int j = 1; j <= maxf; j++){
                printf(" %d",memoi[i][j].val);
            }
            printf("\n");
        }
    }
}

//Using the same functions copy pasted with different names
int Dfw(int a, int b, int * flrs, int n, int maxf){
    int cost = 0;
    int i;

    for(i = 0;flrs[i]<a;i++){} //Skip the people that have a destination thats not inbetween the given floors

    if(b == -1){//Representing infinite floors as -1 on b
        for(;i < n &&flrs[i]<=maxf;i++)     cost += flrs[i] - a;
        return cost;
    }
    
    for(;i < n && flrs[i]<b;i++)            cost += (flrs[i] - a > b - flrs[i]) ? b - flrs[i] : flrs[i] - a;
        
    return cost;
}