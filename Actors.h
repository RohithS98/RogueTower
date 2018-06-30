#ifndef __ACTORCLASS_H__
#define __ACTORCLASS_H__

#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sstream>
using namespace std;

int getRand(int,int);

enum ENEMYTYPE{
	ETYPE1 = 0,
	ETYPE2 = 1,
};

class Actor{
	public:
	
	int health,speed,def,atk,x,y,level,maxhealth,acc,luck;
};

class Player : public Actor{
	public:
	void init(int type = 0,string name = "LOL");
	bool gainXP(int);
	void boostAttack(int);
	void boostDefense(int);
	void boostHealth(int);
	void restoreHealth(int);
	void boostLuck(int);
	void boostAccuracy(int);
	bool getHit(int);
	string getHealthStr();
	string pName;
	int xp,nextxp;
	private:
	int getXP();
	void levelup();
};

class Enemy : public Actor{
	public:
	void init(int type,int level);
	int getType();
	bool getHit(int);
	int type;
	string name;
	bool visible,moved;
	void setVisible();
	void setInvisible();
	private:
	void setStats();
};

#endif
