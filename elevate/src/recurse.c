#include <stdio.h>
#include <stdlib.h>
#include "elevate.h"

int compare(const void* a, const void* b);
int fw(int a, int b, int * flrs, int n);
int minM(int i, int j, int * flrs, int people);
void recursive(int people, int stops, int * reqfloors);

int maxf;//I want to remove this global var as used in memoize, probably will at some point


void recursive(int people, int stops, int * reqfloors){

    int minflor = 0;
    int mincost;

    //We are sorting the array to optimize the floors walked function, since this way we will be able to instantly skip all the passengers
    //with destinations larger than the intended range
    qsort(reqfloors,people,sizeof(int),compare);

    //Since the array is sorted, the highest floor is in the last position of the array
    maxf = reqfloors[people-1];

    //Instantly handling the cenario with no stops
    if(stops == 0){
        printf("No lift stops\nThe minimum cost is: %d\n",Mfw(0,-1,reqfloors,people,maxf));
        return;
    }
    
    //We are initialising the minimum cost with the best-case cenario of the last stop being at the first floor
    mincost = minM(stops,1,reqfloors,people);
    int temp;
    
    //and we are checking all the other cenarios to find the overall best
    for(int i = 2; i <= maxf;i++){  
        if((temp = minM(stops,i,reqfloors,people)) < mincost){
            mincost = temp;
            minflor = i;
        }
    }
    //Finally printing the results while ensuring that stops where made to change the printed message
    if(minflor != 0)    printf("Last stop at floor: %d\nThe minimum cost is: %d\n", minflor,mincost);
    else                printf("No lift stops\nThe minimum cost is: %d\n",mincost);
}

int fw(int a, int b, int * flrs, int n){
    int cost = 0;
    int i;

    for(i = 0; flrs[i] < a; i++){} //Skip the people that have a destination thats lower than the lowest of the requested floors

    if(b == -1){//Representing infinite floors as -1 on b and handling it separately for simplicity
        for(;i < n && flrs[i] <= maxf; i++)     cost += flrs[i] - a;
        return cost;
    }
    
    //If we dont have infinite floors, we start checking from the previously left i up until we hit the max floor
    for(; i < n && flrs[i] < b; i++)            cost += (flrs[i] - a > b - flrs[i]) ? b - flrs[i] : flrs[i] - a;
        
    return cost;
}

int minM(int i, int j, int * flrs, int people){
    int temp;
    if(i == 0) return fw(0,-1,flrs,people); //The endpoint of the recurse loop which will start the unwinding
    
    //We are initialising the minimum cost of the selected floor with the first check 
    int minv = minM(i-1,0,flrs,people) - fw(0,-1,flrs,people) + fw(0,j,flrs,people) + fw(j,-1,flrs,people);
    for(int k = 1; k < j; k++){
        //We are setting temp as the formula inside the if in order for the value to not be recalculated
        //doing it outside the loop would do the same thing but ehh ¯\(ツ)/¯
        if((temp = (minM(i-1,k,flrs,people)-fw(k,-1,flrs,people)+fw(k,j,flrs,people)+fw(j,-1,flrs,people))) < minv){
            minv = temp; 
        }
    }
    return minv;
}

//The comparrison function needed for QSort
int compare(const void* a, const void* b){
   return (*(int*)a - *(int*)b);
}


void printerror(char* error){
    fprintf(stderr, "%s\n" , error);
    exit(1);
}