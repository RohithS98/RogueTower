#ifndef __FLOORMAP_H__
#define __FLOORMAP_H__
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue>
#include "Actors.h"
#include "Logger.h"
#include "LTimer.h"

const int FRAME_PER_MOVE = 7;
const int ATTACKDELAY = 300;

enum BLOCK_TYPE{
	WALL = 0,
	EMPTYOUT = 1,
	EMPTYSEEN = 2,
	CHESTOUT = 3,
	CHESTCLOSED = 4,
	EMPTYBRIGHT = 5,
	WALLEDGEOUT = 7,
	WALLEDGE = 6,
	STAIROUT = 8,
	STAIR = 9,
};

namespace {
	const int WIDTH = 66;
	const int HEIGHT = 46;
	const int MAX_DIST = 90;
}

enum DIRECTION{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
};

struct Room{
	int x,y,w,h;
	
	bool isIntersecting(Room room);
};

class FloorMap{
	
	public:
	
	FloorMap();
	
	void free();
	void setFloor(int floorNo);
	void genMap(int rooms = 5);
	int getBlock(int x, int y);
	int getHeight();
	int getWidth();
	int getPlayerX();
	int getPlayerY();
	void putPlayer();
	void updateView();
	void handleEvent(SDL_Event);
	bool frameDone();
	bool inbounds(int,int);
	bool newData;
	void renderDone();
	int VIEWSIZE;
	Player player;
	vector<Enemy> enemyList;
	int floorNo;
	Logger log;
	
	private:
	
	void fillBlock(int x, int y, int width, int height,int block_type = EMPTYOUT);
	void fillBlock(Room room, int block_type);
	void makePath(int a, int b, int c, int d);
	bool emptyNeighbour(int x, int y);
	void convert(int, int);
	void setBorders();
	bool joinRoom(int,int);
	void checkEnemy(int,int);
	void setEnemyInvisible();
	void addEnemies(int);
	bool moveEnemies();
	bool handleMove();
	bool movePlayer(int dir);
	bool isFree(int x, int y);
	int isEnemy(int,int);
	int damageCalc(Actor a, Actor b);
	void attackEnemy(int);
	int getDisttoPlayer(Enemy e);
	int MIN_ROOM_SIZE, MAX_ROOM_SIZE;
	int floorMap[HEIGHT][WIDTH], mWidth, mHeight, playerX, playerY, troom, nextMove, nextMoveFrame,currentEnemyProcess, floor;
	Room *roomList;
	SDL_Keycode currentKey;
	bool haltFor(int);
};

#endif
