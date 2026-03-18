#include <stdio.h>
#include <stdlib.h>
#include "elevate.h"

int Mcompare(const void* a, const void* b);
int Mfw(int a, int b, int * flrs, int n, int maxf);
int MminM(int i, int j, int * flrs, int people, int ** memoi, int maxf);
void memoize(int people, int stops, int * reqfloors);



void memoize(int people, int stops, int * reqfloors){

    int minflor = 0;
    int mincost;

    
    //We are sorting the array to optimize the floors walked function, since this way we will be able to instantly skip all the passengers
    //with destinations larger than the intended range
    qsort(reqfloors,people,sizeof(int),Mcompare);

    //Since the array is sorted, the highest floor is in the last position of the array
    int maxf = reqfloors[people-1];


    //Initialising the memory for the memoization
    int **memoi;
    memoi = (int **)malloc((stops + 1) * sizeof(int*));
    if(memoi == NULL) printerror((char *)"Failed to allocate memoi array");


    for(int i = 0; i <= stops; i++){
        memoi[i] = (int *)malloc((maxf + 1) * sizeof(int));
        if(memoi[i] == NULL) printerror((char *)"Failed to allocate memoi array");
        for(int j = 0; j <= maxf; j++){
            memoi[i][j] = -1;//Filling the array with -1s to signify non used spaces. Calloc doesnt work since having 0 cost is valid
        }
    }

    int temp = Mfw(0, -1, reqfloors, people, maxf); //Initializing the first row of the table since
    for(int l = 0; l <= maxf; l++)     memoi[0][l] = temp;// The calculation is the same for every value of j, so we can set it like that instantly

    //We are initialising the minimum cost with the best-case cenario of the last stop being at the first floor
    mincost = MminM(stops,1,reqfloors,people,memoi,maxf);
    
    //and we are checking all the other cenarios to find the overall best
    for(int i = 2; i <= maxf;i++){
        if((temp = MminM(stops,i,reqfloors,people,memoi,maxf)) < mincost){
            mincost = temp;
            minflor = i;
        }
    }
    
    if(minflor != 0)printf("Last stop at floor: %d\nThe minimum cost is: %d\n", minflor,mincost);
    else printf("No lift stops\nThe minimum cost is: %d\n",mincost);

    for(int i = 0; i < stops+1;i++)free(memoi[i]);
    free(memoi);
}

//Using the same functions copy pasted with different names since it is requested that the implementation is done in this file exclusively
int Mfw(int a, int b, int * flrs, int n, int maxf){
    int cost = 0;
    int i;

    for(i = 0;i < n &&flrs[i]<a;i++){} //Skip the people that have a destination thats not inbetween the given floors

    if(b == -1){//Representing infinite floors as -1 on b  and handling it separately for simplicity
        for(;i < n &&flrs[i]<=maxf;i++)     cost += flrs[i] - a;
        return cost;
    }
        //If we dont have infinite floors, we start checking from the previously left i up until we hit the max floor

    for(;i < n && flrs[i]<b;i++)            cost += (flrs[i] - a > b - flrs[i]) ? b - flrs[i] : flrs[i] - a;
        
    return cost;
}

int MminM(int i, int j, int * flrs, int people, int ** memoi, int maxf){
    int temp;
    
    if (memoi[i][j] != -1) return memoi[i][j];//Returning the stored value if it has already be calculated

    //We dont need another endpoint as we did in the plain recurse since we have already initialised the first row of the array before calling this function 
    int minv = MminM(i-1, 0, flrs, people, memoi, maxf)- Mfw(0, -1, flrs, people, maxf) + Mfw(0, j, flrs, people, maxf) + Mfw(j, -1, flrs, people, maxf);

    for(int k = 1; k < j; k++){
        //We are setting temp as the formula inside the if in order for the value to not be recalculated
        //doing it outside the loop would do the same thing but ehh ¯\(ツ)/¯
        if((temp = MminM(i-1, k, flrs, people, memoi, maxf) - Mfw(k, -1, flrs, people, maxf) + Mfw(k, j, flrs, people, maxf) + Mfw(j, -1, flrs, people, maxf)) < minv){
            minv = temp; 
        }
    }
    memoi[i][j] = minv; //Storing the value in memory 
    return minv;
}

int MnumFloors(int * flrs, int am){
    int b=0;
    for(int j = 0; j < am; j++)  if(flrs[j] > b) b = flrs[j]; //Finding the highest floor so as to check up to that floor
    return b;
}

//Comparison functon
int Mcompare(const void* a, const void* b){
   return (*(int*)a - *(int*)b);
}