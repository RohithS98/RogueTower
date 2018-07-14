#ifndef __FLOORMAP_H__
#define __FLOORMAP_H__
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include "Actors.h"
#include "Logger.h"
#include "Item.h"
//#include "Utils.h"

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
	STAIRBRIGHT = 9,
	STAIRSEEN = 10,
	CHEST2CLOSED = 11,
};

namespace {
	const int WIDTH = 55;
	const int HEIGHT = 36;
	const int MAX_DIST = 70;
	const int MIN_ROOM_SIZE = 6;
	const int MAX_ROOM_SIZE = 9;
	const std::string blockSpriteLoc = "resources/blocksprite2.png";
	const int MAXROOMNO = 10;
	const int MAX_ENEMY_NO = 2;
	const int VIEWSIZE = 10;
	const int BLUECHESTCHANCE = 5;
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
	void setFloor(Logger &log, int floorNo);
	void genMap(Graphics &graphics,int rooms = MAXROOMNO);
	int getChestNo();
	int getBlock(int x, int y);
	int getHeight();
	int getWidth();
	Vector2 getPlayerPos();
	bool inbounds(int,int);
	Vector2 putPlayer();
	void setSprites();
	void drawMap(Graphics &graphics);
	void drawViewCone(Graphics &graphics);
	void drawEnemy(Graphics &graphics);
	void drawItems(Graphics &graphics);
	void setPlayerPos(Vector2 ppos);
	SDL_Rect* getSpriteRect(int sprite);
	void updateView();
	void handleMove(Player &player, Logger &log, SDL_Keycode key, int currentFrame, Graphics &graphics);

	Vector2 playerPos;
	std::vector<Enemy> enemyList;
	std::vector<Item> itemList;
	int floorNo, currentEnemyProcess;

	private:

	int isItem(int x, int y);
	int highlight(int);
	bool movePlayer(Logger &log, int dir, Player &player);
	void openChest(int posx, int posy, Player &player);
	void fillBlock(int x, int y, int width, int height,int block_type = EMPTYOUT);
	void fillBlock(Room room, int block_type);
	void makePath(int a, int b, int c, int d);
	bool emptyNeighbour(int x, int y);
	void convert(int, int);
	void setBorders();
	bool joinRoom(int,int);
	bool isWalkable(int,int);
	void checkEnemy(int,int);
	void setEnemyInvisible();
	int getEnemyLevel();
	int getEnemyType();
	void addEnemies(Graphics &graphics);
	void moveEnemies(Logger &log, Player &player);
	bool isFree(int x, int y);
	int isEnemy(int,int);
	int damageCalc(Actor &a, Actor &b);
	void attackEnemy(Player &player, Logger &log, int eno);
	int getDisttoPlayer(Enemy &e);
	int getXP(Enemy &e);
	void putStairs();
	void goToNextLevel(Logger &log, Graphics &graphics);
	int floorMap[HEIGHT][WIDTH], mWidth, mHeight, troom;
	Room *roomList;
	SDL_Texture* blockSprites;
	std::vector<SDL_Rect> tileClip;
};

#endif
