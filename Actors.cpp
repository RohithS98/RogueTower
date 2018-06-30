#include "Actors.h"
#include "Utils.h"

using namespace std;

int min(int a,int b);

int Player::getXP(){
	return 2*level*log10(level) + 10;
}

void Player::boostAttack(int a){
	atk += a;
}

void Player::boostDefense(int a){
	def += a;
}

void Player::boostHealth(int a){
	maxhealth += a;
}

void Player::boostLuck(int a){
	luck += a;
}

void Player::boostAccuracy(int a){
	acc += a;
}

void Player::restoreHealth(int a){
	health = min(health + a,maxhealth);
}

void Player::levelup(){
	level++;
	atk += getRand(2,3);
	def += getRand(2,3);
	maxhealth += getRand(3,5);
	health = min(health + 2,maxhealth);
	luck += getRand(0,1);
}

bool Player::gainXP(int exp){
	xp += exp, nextxp -= exp;
	if(nextxp <= 0){
		levelup();
		xp = -nextxp;
		nextxp = getXP();
		return true;
	}
	return false;
}

void Player::init(int type,string name){
	pName = name;
	maxhealth = getRand(17,20);
	health = maxhealth;
	atk = getRand(3,5);
	def = getRand(3,5);
	acc = 90;
	luck = 1;
	speed = 1;
	x = 0;
	y = 0;
	level = 1;
	xp = 0;
	nextxp = getXP();
}	

string Player::getHealthStr(){
	stringstream s1;
	s1.str("");
	s1<<health<<" / "<<maxhealth;
	return s1.str();
}

void Enemy::init(int etype, int elevel){
	type = etype;
	level = elevel;
	x = 0;y = 0 ;
	setStats();
	health = maxhealth;
}

int Enemy::getType(){
	return type;
}

void Enemy::setStats(){
	int basea,based,basemh,bases,baseac,basel;
	switch(type){
		case ETYPE1:
			basea = 2;based = 1;basemh = 5;bases = 1;baseac = 70;basel = 1;name = "Blob";break;
		case ETYPE2:
			basea = 3;based = 2;basemh = 4;bases = 1;baseac = 50;basel = 1;name = "Blob2";break;
	}
	atk = basea + (level-1)*getRand(1,2);
	def = based + (level-1)*getRand(1,2);
	maxhealth = basemh + (level-1)*getRand(2,4);
	acc = baseac + (level/2)*getRand(0,1) + getRand(0,1)*(level/2);
}

bool Enemy::getHit(int damage){
	health -= damage;
	if(health <= 0){
		return true;
	}
	return false;
}

void Enemy::setVisible(){
	visible = true;
}

void Enemy::setInvisible(){
	visible = false;
}

bool Player::getHit(int damage){
	health -= damage;
	if(health <= 0){
		return true;
	}
	return false;
}