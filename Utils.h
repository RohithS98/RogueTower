#ifndef __UTILS_H__
#define __UTILS_H__

void swap(int &a, int &b);

int min(int a, int b);

int max(int a, int b);

int abs(int a);

int getRand(int a, int b);

bool isAdj(int x1,int y1, int x2, int y2);

float dist(int x1,int y1, int x2, int y2);

int min(float a[],int n);

class Vector2{
	public:
	int x,y;
	Vector2():x(0),y(0){}
	Vector2(int x, int y):x(x),y(y){}
	Vector2 zero(){
		return Vector2(0,0);
	}
};

namespace global{
	const int TILE_SIZE = 15;
}
#endif
