#include <stdlib.h>
#include <math.h>
#include "Utils.h"

using namespace std;

//Swaps 2 variables
void swap(int &a, int &b){
	int temp = a;
	a = b;
	b = temp;
}

//Returns min of two values
int min(int a, int b){
	return a < b ? a : b;
}

//Returns max of 2 values
int max(int a, int b){
	return a > b ? a : b;
}

//Returns absoulte value
int abs(int a){
	return a > 0 ? a : (-1)*a;
}

//Returns random integer from a to b(inclusive)
int getRand(int a, int b){
	return rand()%(b-a+1) + a;
}

//Checks if 2 points are adjacent
bool isAdj(int x1,int y1, int x2, int y2){
	if(x1==x2)
		return abs(y1-y2) == 1;
	else if(y1==y2)
		return abs(x1-x2) == 1;
	else
		return false;
}

//Returns distance fo 2 points
float dist(int x1,int y1, int x2, int y2){
	return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

//Returns index of minimum value in a array
int min(float a[],int n){
	int mi = 0;
	for(int i = 1 ; i < n ; i++){
		if(a[i] < a[mi]){
			mi = i;
		}
	}
	return mi;
}
