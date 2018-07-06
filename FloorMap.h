#ifndef __FLOORMAP_H__
#define __FLOORMAP_H__
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include "Actors.h"
#include "Logger.h"
#include "Utils.h"

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
	const int MIN_ROOM_SIZE = 7;
	const int MAX_ROOM_SIZE = 10;
	const std::string blockSpriteLoc = "resources/blocksprite2.png";
	const int TILE_SIZE = 15;
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
	FloorMap(Graphics &graphics);
	~FloorMap();
	void free();
	void setFloor(int floorNo);
	void genMap(int rooms = 5);
	int getChestNo();
	int getBlock(int x, int y);
	int getHeight();
	int getWidth();
	Vector2 getPlayerPos();
	bool inbounds(int,int);
	Vector2 putPlayer();
	void setSprites();
	void render(Graphics &graphics);
	SDL_Rect* getSpriteRect(int sprite);
	
	void updateView(int x, int y);	
	//void handleEvent(SDL_Event);	
	//bool frameDone();	
	bool newData;	
	void renderDone();	
	int VIEWSIZE;
	Vector2 playerPos;	
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
	//void addEnemies(int);
	//bool moveEnemies();
	//bool handleMove();
	//bool movePlayer(int dir);
	bool isFree(int x, int y);
	int isEnemy(int,int);
	int damageCalc(Actor a, Actor b);
	//void attackEnemy(int);
	//int getDisttoPlayer(Enemy e);
	int floorMap[HEIGHT][WIDTH], mWidth, mHeight, playerX, playerY, troom, nextMove, nextMoveFrame,currentEnemyProcess;
	Room *roomList;
	//SDL_Keycode currentKey;	
	SDL_Texture* blockSprites;
	vector<SDL_Rect> tileClip;
};

#endif
