#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "elevate.h"



void brute(int people, int maxstops, int * reqfloors){

    //Again, sorting the array with the requested floors in order to optimize fw
    qsort(reqfloors,people,sizeof(int),compare);

    //And again storing the maximum requested floor in a variable
    //Could have directly used reqfloors[people-1], but ¯\(ツ)/¯
    int maxf = reqfloors[people-1];
    MinComb min;//Using a struct located in elevate.h for the minimum path and cost
    //the struct could not be located here since it is required in elevate.h's deifinitions of the dunctions

    //Instantly handling the scenario with no stops
    if(maxstops == 0){
        printf("No lift stops\nThe minimum cost is: %d\n",Mfw(0,-1,reqfloors,people,maxf));
        return;
    }

    //Alocating the array where the path is stored 
    min.path = (int *)malloc((maxstops+1)*sizeof(int));
    if(min.path == NULL) printerror((char *)"Failed to allocate min.path array");

    for(int i = 0; i <= maxstops; i++)  min.path[i] = -1;
    min.cost = -1;

    
    //Allocating the array where for the floor mapping 
    int * floormap = (int *)malloc((maxstops+1) * sizeof(int));
    if(floormap == NULL) printerror((char *)"Failed to allocate floormap array");
    for(int stops = 0; stops <= maxstops; stops++){
        for(int i = 0; i < maxstops; i++)   floormap[i] = 0;
        
        findpath(1,0,stops,reqfloors,people,floormap,maxf,&min);
    }


    //There is no need to check for the optimal solution stopping on the 0th floor since always, if there is a passenger it will reduce the cost to drop him off at his requested floor
    printf("Lift stops are: %d", min.path[0]);
    for(int i = 1; min.path[i] != -1 ; i++) printf(" %d" , min.path[i]);
    printf("\nThe minimum cost is: %d\n",min.cost);

}


void findpath(int start, int count, int max, int * floors, int n, int * path,int maxf, MinComb * best){
    /*
    It starts doing a recursive process where it selects all the possible unique combinations. 
    It starts of by the inputed maximum floor, then goes and selects the first available next stop
    up until it selects maxstops floors. After that it unselects the last picked floor and picks the 
    next avaialble floor. After it finishes scanning the possibilities for the mastops floor, it unpicks
    the maxstops-1 selected floor, picks the next available one, and so on 
    */

    //If we have not exceeded the maximum allowed picked floors (check needed for the recursion)
    //It starts from 0 from the call from the main function (brute)
    if(count == max){ 

        int curmin = 0; //Current minimum cost
        int i;
        int prev = 0;  //Tracks the previous stop made by the elevator to calculate the cost in between the previous and this floor

        for(i = 0; i < max; i++){//Probably could start checking from i = 1 and discard prev. Will hopefully check it at some point and not ignore it cause of GO
            curmin += Mfw(prev,path[i],floors,n,maxf);
            prev = path[i];
        }

        curmin += Mfw(prev, -1, floors, n, maxf);// Calculationg the cost needed for everyone that wants to go higher than the last stop of the elevator
        if(curmin < best->cost || best->cost == -1){ //If we found a new minimum, it updates both the overall minimum cost as well as the minimum path 
            best->cost = curmin;
            for(int j = 0; j < max; j++) {
                best->path[j] = path[j];
            }
            best->path[max] = -1; 
        }
        
        return;
    }

    for( int i = start; i <= maxf; i++){
        path[count] = i; //When this recursion finished, it picks the next possible floor, and starts the new path 
        findpath(i + 1 , count + 1, max, floors,n,path,maxf,best);
    }
    return;
}
