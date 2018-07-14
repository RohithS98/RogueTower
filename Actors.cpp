#include "Actors.h"

using namespace std;

Actor::Actor(){
	std::cout << "Actor Created" << std::endl;
}

Actor::~Actor(){
	std::cout << "Actor Deleted" << std::endl;
}

void Actor::setSprite(Graphics &graphics, const std::string &filePath, int sourceX,int sourceY, int width, int height){
	sourceRect.x = sourceX;
	sourceRect.y = sourceY;
	sourceRect.w = width;
	sourceRect.h = height;
	spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(),
												graphics.loadImage(filePath));
}

void Player::draw(Graphics &graphics){
	SDL_Rect destRect = {y*global::TILE_SIZE,x*global::TILE_SIZE,global::TILE_SIZE,global::TILE_SIZE};
	graphics.blitSurface( spriteSheet, &sourceRect, &destRect );
}

void Enemy::draw(Graphics &graphics){
	float halfScale = (ActorConst::enemyScale - 1.0)/2;
	SDL_Rect destRect = {(y-halfScale)*global::TILE_SIZE,(x-halfScale)*global::TILE_SIZE,global::TILE_SIZE*ActorConst::enemyScale,global::TILE_SIZE*ActorConst::enemyScale};
	graphics.blitSurface( spriteSheet, &sourceRect, &destRect );
}

Player::Player(){
	std::cout << "Player Created" << std::endl;
}

Player::~Player(){
	std::cout << "Player Destroyed" << std::endl;
}

int Player::getXP(){
	return 3*level*log10(level) + 15;
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
	luck = min(10,luck+a);
}

void Player::boostAccuracy(int a){
	acc += a;
}

void Player::restoreHealth(int a){
	health = min(health + a,maxhealth);
}

void Player::useItem(Item item, Logger &log){
	switch(item.type){
		case atkUp: boostAttack(item.strength);log.logStatA();break;
		case defUp: boostDefense(item.strength);log.logStatD();break;
		case luckUp: boostLuck(item.strength);log.logStatL();break;
		case potions:case potionl: restoreHealth(item.strength);log.logHeal(item.strength);break;
		case healthUp: boostHealth(item.strength);log.logStatH();break;
	}
}

void Player::levelup(){
	level++;
	atk += getRand(2,3);
	def += getRand(2,3);
	maxhealth += getRand(3,5);
	health = min(health + 2,maxhealth);
	luck = min(10,luck+getRand(0,1));
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

std::string Player::getString(int x){
	stringstream st1;
	st1.str("");
	st1<<x;
	return st1.str();
}

void Player::renderStat(Graphics &graphics){
	TTF_Font* gFont = graphics.loadFont(ActorConst::fontName,ActorConst::size);
	SDL_Rect bounds = {0,0,210,220};
	graphics.drawRect(bounds,0xBB,0xBB,0xBB);
	int i = 0;
	graphics.blitTextCenterX(gFont,"PLAYER:",ActorConst::statnamecol,105, 25*i++);
	graphics.blitTextCenterX(gFont,"LEVEL:",ActorConst::statnamecol,105, 25*i++);
	graphics.blitTextCenterX(gFont,"HEALTH:",ActorConst::statnamecol,105, 25*i++);
	graphics.blitTextCenterX(gFont,"ATTACK:",ActorConst::statnamecol,105, 25*i++);
	graphics.blitTextCenterX(gFont,"DEFENSE:",ActorConst::statnamecol,105, 25*i++);
	graphics.blitTextCenterX(gFont,"XP:",ActorConst::statnamecol,105, 25*i++);
	graphics.blitTextCenterX(gFont,"NEXTLVL:",ActorConst::statnamecol,105, 25*i++);
	i = 0;
	graphics.blitTextCenterX(gFont,pName,ActorConst::statvalcol,105,25*i++,105);
	graphics.blitTextCenterX(gFont,getString(level),ActorConst::statvalcol,105,25*i++,105);
	graphics.blitTextCenterX(gFont,getHealthStr(),ActorConst::statvalcol,105, 25*i++,105);
	graphics.blitTextCenterX(gFont,getString(atk),ActorConst::statvalcol,105, 25*i++,105);
	graphics.blitTextCenterX(gFont,getString(def),ActorConst::statvalcol,105, 25*i++,105);
	graphics.blitTextCenterX(gFont,getString(xp),ActorConst::statvalcol,105, 25*i++,105);
	graphics.blitTextCenterX(gFont,getString(nextxp),ActorConst::statvalcol,105, 25*i++,105);
}

void Player::init(Graphics &graphics, int type,string name){
	setSprite(graphics,"resources/blocksprite2.png",300,0,100,100);
	pName = name;
	maxhealth = getRand(17,20);
	health = maxhealth;
	atk = getRand(3,5);
	def = getRand(3,5);
	acc = 95;
	luck = 1;
	speed = 1;
	x = 0;
	y = 0;
	level = 1;
	xp = 0;
	nextxp = getXP();
}

void Player::setPosition(Vector2 vec){
	x = vec.x;
	y = vec.y;
}

Vector2 Player::getPosition(){
	return Vector2(x,y);
}

string Player::getHealthStr(){
	stringstream s1;
	s1.str("");
	s1<<health<<" / "<<maxhealth;
	return s1.str();
}

Enemy::Enemy(){
	std::cout << "Enemy Created" << std::endl;
}

Enemy::~Enemy(){
	std::cout << "Enemy Destroyed" << std::endl;
}

void Enemy::init(Graphics &graphics, int etype, int elevel){
	setSprite(graphics,"resources/enemysprite2.png",0+etype*100,0,100,100);
	type = etype;
	level = elevel;
	x = 0;
	y = 0;
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
			basea = 2;based = 1;basemh = 5;bases = 1;baseac = 85;basel = 1;name = "Blob";break;
		case ETYPE2:
			basea = 3;based = 2;basemh = 4;bases = 1;baseac = 70;basel = 1;name = "Ghost";break;
	}
	atk = basea + (level-1)*getRand(1,2);
	def = based + (level-1)*getRand(1,2);
	maxhealth = basemh + (level-1)*getRand(2,4);
	acc = baseac + (level/2)*getRand(0,1) + getRand(0,1)*(level/2);
	luck = basel + ((level-1)/2)*getRand(0,1);
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
