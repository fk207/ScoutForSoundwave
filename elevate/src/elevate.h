#ifndef ELEVATE_H   
#define ELEVATE_H

typedef struct MinCombS{
    int cost;
    int * path;
}MinComb;

int compare(const void* a, const void* b);
int minM(int i, int j, int * flrs, int people);
void recursive(int people, int stops, int * reqfloors);

int Mcompare(const void* a, const void* b);
int Mfw(int a, int b, int * flrs, int n, int maxf);
int MminM(int i, int j, int * flrs, int people, int ** memoi, int maxf);
void memoize(int people, int stops, int * reqfloors);

void brute(int people, int maxstops, int * reqfloors);
void findpath(int start, int count, int max, int * floors, int n, int * path,int maxf, MinComb * best);


void dp(int people, int stops, int * reqfloors, int debug);

void printerror(char* error);

#endif