#ifndef __ACTORCLASS_H__
#define __ACTORCLASS_H__

#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include "Graphics.h"

class Graphics;


int getRand(int,int);

enum ENEMYTYPE{
	ETYPE1 = 0,
	ETYPE2 = 1,
};

class Actor{
	public:
	int health,speed,def,atk,x,y,level,maxhealth,acc,luck;
	void setSprite(Graphics &graphics, const std::string &filePath, int sourceX,
					int sourceY, int width, int height);
	void draw(Graphics &graphics, int x, int y, int mw, int mh);
	protected:
	SDL_Rect sourceRect;
	SDL_Texture* spriteSheet;
};

class Player : public Actor{
	public:
	void init(Graphics &graphics, int type = 0,std::string name = "LOL");
	bool gainXP(int);
	void boostAttack(int);
	void boostDefense(int);
	void boostHealth(int);
	void restoreHealth(int);
	void boostLuck(int);
	void boostAccuracy(int);
	bool getHit(int);
	std::string getHealthStr();
	std::string pName;
	int xp,nextxp;
	private:
	int getXP();
	void levelup();
};

class Enemy : public Actor{
	public:
	void init(Graphics &graphics, int type, int level);
	int getType();
	bool getHit(int);
	int type;
	std::string name;
	bool visible,moved;
	void setVisible();
	void setInvisible();
	private:
	void setStats();
};

#endif
