#ifndef __ACTORCLASS_H__
#define __ACTORCLASS_H__

#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include "Graphics.h"
#include "Utils.h"
#include "Item.h"
#include "Logger.h"

class Graphics;


int getRand(int,int);

enum ENEMYTYPE{
	ETYPE1 = 0,
	ETYPE2,
	ETYPE3,
};

namespace ActorConst{
	const std::string fontName = "resources/VT323-Regular.ttf";
	const int size = 24;
	const SDL_Color statnamecol = {0x0,0x0,0x0,0xFF};
	const SDL_Color statvalcol = {0x77,0x0,0x0,0xFF};
	const float enemyScale = 1.6;
	const int SPRITES_PER_LINE = 5;
}

class Actor{
	public:
	Actor();
	~Actor();
	int health,speed,def,atk,x,y,level,maxhealth,acc,luck;
	protected:
	SDL_Rect sourceRect;
};

class Player : public Actor{
	public:
	Player();
	~Player();
	void init(Graphics &graphics, int type = 0,std::string name = "LOL");
	void setPosition(Vector2 vec);
	Vector2 getPosition();
	bool gainXP(int);
	void useItem(Item &item, Logger &log);
	bool getHit(int);
	void renderStat(Graphics &graphics);
	std::string getHealthStr();
	std::string pName;
	int xp,nextxp;
	void draw(Graphics &graphics);
	static void freeSprites();
	private:
	std::string getString(int x);
	int getXP();
	void levelup();
	void boostAttack(int);
	void boostDefense(int);
	void boostHealth(int);
	void restoreHealth(int);
	void boostLuck(int);
	void boostAccuracy(int);
	void setSprite(Graphics &graphics, const std::string &filePath, int sourceX,
					int sourceY, int width, int height);
	static bool isSpriteLoaded;
	static SDL_Texture* spriteSheet;
};

class Enemy : public Actor{
	public:
	Enemy();
	~Enemy();
	void init(int type, int level);
	int getType();
	bool getHit(int);
	int type;
	std::string name;
	bool visible,moved;
	void setVisible();
	void setInvisible();
	void draw(Graphics &graphics);
	static void freeSprites();
	private:
	void setStats();
	void setSprite(Graphics &graphics, const std::string &filePath, int sourceX,
					int sourceY, int width, int height);
	static bool isSpriteLoaded;
	static SDL_Texture* spriteSheet;
};

#endif
