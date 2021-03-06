#include "FloorMap.h"
#include <queue>

//Check if 2 rooms intersect
bool Room::isIntersecting(Room room){
	if (x >= room.x + room.w+5 || w + x <= room.x-5)
        return false;

     if (y >= room.y + room.h+5 || y + h <= room.y-5)
        return false;

    return true;
}

FloorMap::FloorMap(){
	#ifdef DEBUG
	std::cout<<"Map Created"<<std::endl;
	#endif
}

//Initialize floor map class, set floor to 1, set sprites.
FloorMap::FloorMap(Graphics &graphics){
	//int seed = 1531502359;
	int seed = time(NULL);
	std::cout<<"Seed: "<<seed<<std::endl;
	srand(seed);
	roomList = NULL;
	playerPos = Vector2(-1,-1);
	floorNo = 1;
	currentEnemyProcess = -1;
	blockSprites = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(blockSpriteLoc));
	setSprites();
}

FloorMap::~FloorMap(){
	free();
	#ifdef DEBUG
	std::cout<<"Map Destroyed"<<std::endl;
	#endif
}

void FloorMap::free(){
	if(roomList!=NULL)
		delete[] roomList;
	enemyList.clear();
	itemList.clear();
}

//Set the floor number and log it
void FloorMap::setFloor(Logger &log, int floorNo){
	this->floorNo = floorNo;
	log.logNewFloor(this->floorNo);
}

//Generate a new map
void FloorMap::genMap(Graphics &graphics,int rooms){
	free();
	mWidth = WIDTH; mHeight = HEIGHT;
	fillBlock(0,0,mWidth,mHeight,WALL);
	roomList = new Room[rooms];
	bool flag; troom = 0; int k = 0;

	for(int i = 0; i < rooms && k < 1000;){
		Room tempRoom;
		tempRoom.w = getRand(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
		tempRoom.h = getRand(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
		tempRoom.x = getRand(2, mWidth - tempRoom.w - 2);
		tempRoom.y = getRand(2, mHeight - tempRoom.h - 2);
		flag = true;
		k += 1;
		for(int j = 0; j < i; j++){
			if(tempRoom.isIntersecting(roomList[j])){
				flag = false;
				break;
			}
		}
		if(flag){
			roomList[i++] = tempRoom;
			troom++; k = 0;
		}
	}

	for(int i = 0; i < troom; i++){
		fillBlock(roomList[i],EMPTYOUT);
	}

	int chestNo = getChestNo();
	int mapList[chestNo][2];
	// TODO : Spawn Chests inside rooms only
	for(int i = 0; i < chestNo ;){
		int tx = getRand(5,mHeight-5), ty = getRand(5,mWidth-5);
		if( emptyNeighbour(tx,ty) ){
			mapList[i][0] = tx;
			mapList[i][1] = ty;
			i++;
		}
	}

	for(int i = 0; i < troom - 1; i++){
		joinRoom(i,i+1);
	}

	for(int i = 0; i < 8 ;){
		if(joinRoom(getRand(0,troom-1),getRand(0,troom-1))){
			i++;
		}
	}

	for(int i = 0 ; i < chestNo ; i++){
		floorMap[mapList[i][0]][mapList[i][1]] = CHESTOUT;
	}
	setBorders();
	putStairs();
	addEnemies();
}

int FloorMap::getChestNo(){
	if(getRand(1,100) < 5)
		return getRand(4,6);
	else if(getRand(1,100) < 15)
		return 0;
	else
		return getRand(1,3);
}

int FloorMap::getBlock(int x, int y){
	return floorMap[x][y];
}

int FloorMap::getWidth(){
	return mWidth;
}

int FloorMap::getHeight(){
	return mHeight;
}

Vector2 FloorMap::getPlayerPos(){
	return playerPos;
}

//Check if point is in bounds of map
bool FloorMap::inbounds(int px, int py){
	if(py < mWidth && px >=0 && px < mHeight && py >=0){
		return true;
	}
	return false;
}

//Puts the player in a random room
Vector2 FloorMap::putPlayer(){
	int rno = getRand(0,troom-1);
	bool flag = true;
	int tx = roomList[rno].y+roomList[rno].h/2;
	int ty = roomList[rno].x+roomList[rno].w/2;
	while(flag){
		tx = getRand(roomList[rno].y+1,roomList[rno].y+roomList[rno].h-1);
		ty = getRand(roomList[rno].x+1,roomList[rno].x+roomList[rno].w-1);
		if(isFree(tx,ty)){
			flag = false;
		}
	}
	playerPos = Vector2(tx,ty);
	updateView();
	return getPlayerPos();
}

//Sets the sprites for the map
void FloorMap::setSprites(){
	SDL_Rect r = {0,0,100,100};
	SDL_Rect r2 = {100,0,100,100};
	SDL_Rect r4 = {200,0,100,100};
	SDL_Rect r5 = {400,0,100,100};
	SDL_Rect r6 = {500,0,100,100};
	SDL_Rect r10 = {0,100,100,100};
	SDL_Rect r9 = {100,100,100,100};
	SDL_Rect r11 = {200,100,100,100};
	tileClip.push_back(r);		//WALL
	tileClip.push_back(r);		//EMPTYOUT
	tileClip.push_back(r2);		//EMPTYSEEN
	tileClip.push_back(r);		//CHESTOUT
	tileClip.push_back(r4);		//CHESTCLOSED
	tileClip.push_back(r5);		//EMPTYBRIGHT
	tileClip.push_back(r6);		//WALLEDGE
	tileClip.push_back(r);		//WALLEDGEOUT
	tileClip.push_back(r);		//STAIROUT
	tileClip.push_back(r9);		//STAIRBRIGHT
	tileClip.push_back(r10);	//STAIRSEEN
	tileClip.push_back(r11);	//CHEST2CLOSED
}

//Draws the base map on screen
void FloorMap::drawMap(Graphics &graphics){
	int ts = global::TILE_SIZE;
	for(int i = 0; i < mHeight; i++){
		for(int j = 0; j < mWidth ; j++){
			SDL_Rect destRect = {j*ts,i*ts,ts,ts};
			graphics.blitSurface(this->blockSprites,getSpriteRect(floorMap[i][j]),
									&destRect);
		}
	}
}

//Draws the viewcone(using BFS)
void FloorMap::drawViewCone(Graphics &graphics){
	int ts = global::TILE_SIZE;
	std::queue<int> q1,q2,q3;
	q1.push(playerPos.x);q2.push(playerPos.y);q3.push(VIEWSIZE);
	bool visited[mHeight][mWidth];
	for(int i = 0; i < mHeight ; i++){
		for(int j = 0; j < mWidth ; j++){
			visited[i][j] = false;
		}
	}
	visited[playerPos.x][playerPos.y] = true;
	while(!q1.empty()){
		int t1 = q1.front(),t2 = q2.front(), t3 = q3.front();
		q1.pop();q2.pop();q3.pop();
		if(t3-1 && !visited[t1-1][t2] && floorMap[t1-1][t2]!=WALL && floorMap[t1-1][t2]!=WALLEDGE){
			q1.push(t1-1);q2.push(t2);q3.push(t3-1);visited[t1-1][t2] = true;
		}
		if(t3-1 && !visited[t1+1][t2] && floorMap[t1+1][t2]!=WALL&& floorMap[t1+1][t2]!=WALLEDGE){
			q1.push(t1+1);q2.push(t2);q3.push(t3-1);visited[t1+1][t2] = true;
		}
		if(t3-1 && !visited[t1][t2+1] && floorMap[t1][t2+1]!=WALL && floorMap[t1][t2+1]!=WALLEDGE){
			q1.push(t1);q2.push(t2+1);q3.push(t3-1);visited[t1][t2+1] = true;
		}
		if(t3-1 && !visited[t1][t2-1] && floorMap[t1][t2-1]!=WALL && floorMap[t1][t2-1]!=WALLEDGE){
			q1.push(t1);q2.push(t2-1);q3.push(t3-1);visited[t1][t2-1] = true;
		}
		SDL_Rect destRect = {t2*ts,t1*ts,ts,ts};
		graphics.blitSurface(this->blockSprites,
								getSpriteRect(highlight(floorMap[t1][t2])),&destRect);
	}
}

void FloorMap::drawEnemy(Graphics &graphics){
	for(int i = 0 ; i < enemyList.size(); i++){
		if(enemyList[i].visible)
			enemyList[i].draw(graphics);
	}
}

void FloorMap::drawItems(Graphics &graphics){
	for(int i = 0 ; i < itemList.size(); i++){
		itemList[i].draw(graphics);
	}
}

void FloorMap::setPlayerPos(Vector2 ppos){
	playerPos = ppos;
}

SDL_Rect* FloorMap::getSpriteRect(int sprite){
	return &tileClip[sprite];
}

void FloorMap::updateView(){
	std::queue<int> q1,q2,q3;
	q1.push(playerPos.x);q2.push(playerPos.y);q3.push(VIEWSIZE);
	bool visited[mHeight][mWidth];
	for(int i = 0; i < mHeight ; i++){
		for(int j = 0; j < mWidth ; j++){
			visited[i][j] = false;
		}
	}
	setEnemyInvisible();
	visited[playerPos.x][playerPos.y] = true;
	while(!q1.empty()){
		int t1 = q1.front(),t2 = q2.front(), t3 = q3.front();
		q1.pop();q2.pop();q3.pop();
		if(t3-1 && !visited[t1-1][t2] && floorMap[t1-1][t2]!=WALL && floorMap[t1-1][t2]!=WALLEDGE){
			q1.push(t1-1);q2.push(t2);q3.push(t3-1);visited[t1-1][t2] = true;
		}
		else if(floorMap[t1-1][t2]==WALLEDGE){
			convert(t1-1,t2);
		}
		if(t3-1 && !visited[t1+1][t2] && floorMap[t1+1][t2]!=WALL&& floorMap[t1+1][t2]!=WALLEDGE){
			q1.push(t1+1);q2.push(t2);q3.push(t3-1);visited[t1+1][t2] = true;
		}
		else if(floorMap[t1+1][t2]==WALLEDGE){
			convert(t1+1,t2);
		}
		if(t3-1 && !visited[t1][t2+1] && floorMap[t1][t2+1]!=WALL && floorMap[t1][t2+1]!=WALLEDGE){
			q1.push(t1);q2.push(t2+1);q3.push(t3-1);visited[t1][t2+1] = true;
		}
		else if(floorMap[t1-1][t2+1]==WALLEDGE){
			convert(t1,t2+1);
		}
		if(t3-1 && !visited[t1][t2-1] && floorMap[t1][t2-1]!=WALL && floorMap[t1][t2-1]!=WALLEDGE){
			q1.push(t1);q2.push(t2-1);q3.push(t3-1);visited[t1][t2-1] = true;
		}
		else if(floorMap[t1][t2-1]==WALLEDGE){
			convert(t1,t2-1);
		}
		convert(t1,t2);
		checkEnemy(t1,t2);
	}
}

void FloorMap::handleMove(Player &player, Logger &log, SDL_Keycode key, int currentFrame, Graphics &graphics){
	setPlayerPos(player.getPosition());
	//printf("CurrentFrame : %d\n",currentFrame);
	if(currentEnemyProcess == -1 && currentFrame == 0){
		bool ret = false;
		switch(key){
			case SDLK_w:case SDLK_UP:
				ret = movePlayer(log,UP,player);
				//printf("UP\n");
				break;
			case SDLK_a:case SDLK_LEFT:
				ret = movePlayer(log,LEFT,player);
				//printf("LEFT\n");
				break;
			case SDLK_s:case SDLK_DOWN:
				ret = movePlayer(log,DOWN,player);
				//printf("DOWN\n");
				break;
			case SDLK_d:case SDLK_RIGHT:
				ret = movePlayer(log,RIGHT,player);
				//printf("RIGHT\n");
				break;
		}
		int t;
		if(floorMap[player.x][player.y] == STAIRSEEN){
			this->goToNextLevel(log,graphics);
			player.setPosition(this->putPlayer());
			return;
		}
		else if(t = isItem(player.x,player.y)){
			t--;
			player.useItem(itemList[t],log);
			itemList.erase(itemList.begin()+t);

		}
		if(ret){
			currentEnemyProcess = 0;
		}
	}
	else if(currentEnemyProcess != -1 && currentFrame <= 0){
		moveEnemies(log,player);
	}
}

void FloorMap::freeSprites(){
	SDL_DestroyTexture(blockSprites);
	Item::freeSprites();
	Enemy::freeSprites();
}

int FloorMap::isItem(int x, int y){
	if(isWalkable(x,y)){
		for(int i = 0; i < itemList.size() ; i++){
			if(x == itemList[i].x && y == itemList[i].y){
				return i+1;
			}
		}
	}
	return 0;
}

int FloorMap::highlight(int tiletype){
	switch(tiletype){
		case EMPTYSEEN:return EMPTYBRIGHT;
		case STAIRSEEN:return STAIRBRIGHT;
		default: return tiletype;
	}
}

/*
					atkUp	defUp	luckUp	potionSmall		potionLarge		healthUp
	Normal Chest :	10%		10%		5%		50%				15%				10%
	Blue Chests	 :	15%		15%		25%		5%				25%				15%
*/
void FloorMap::openChest(int posx, int posy, Player &player){
	int cType = floorMap[posx][posy] == CHEST2CLOSED ? 1 : 0;
	int level = 1, type;
	if(cType){
		int t = getRand(1,100);
		if(t <= 10)
			type = 0;
		else if(t <= 20)
			type = 1;
		else if(t <= 25)
			type = 2;
		else if(t <= 75)
			type = 3;
		else if(t <= 90){
			type = 4;
			level = 2;
		}
		else
			type = 5;
	}
	else{
		int t = getRand(1,100);
		if(t <= 15)
			type = 0;
		else if(t <= 30)
			type = 1;
		else if(t <= 55)
			type = 2;
		else if(t <= 60)
			type = 3;
		else if(t <= 85){
			type = 4;
			level = 2;
		}
		else
			type = 5;
	}

	switch(type){
		case 0:case 1:case 2:case 5:
		level *= getRand(4,7);break;
		default:
		level *= 0.1*player.maxhealth ;break;
	}
	this->floorMap[posx][posy] = EMPTYSEEN;
	this->itemList.push_back(Item(posx,posy,type,level));
}

bool FloorMap::movePlayer(Logger &log, int direction, Player &player){
	bool ret = false;
	int t = -1,posx = playerPos.x, posy = playerPos.y;
	switch(direction){
		case UP:
			posx--;break;
		case LEFT:
			posy--;break;
		case DOWN:
			posx++;break;
		case RIGHT:
			posy++;break;
	}
	if( isFree(posx,posy) ){
		playerPos = Vector2(posx,posy);
		player.x = posx;
		player.y = posy;
		ret = true;
	}
	else if( t = isEnemy(posx,posy) ){
		ret = true;
		attackEnemy(player,log,t);
	}
	else if( floorMap[posx][posy] == CHESTCLOSED || floorMap[posx][posy] == CHEST2CLOSED){
		ret = true;
		openChest(posx,posy,player);
	}
	updateView();
	return ret;
}

void FloorMap::fillBlock(int x, int y, int width, int height,int block_type){
	for(int i = 0 ; i < height ; i++){
		for(int j = 0; j < width; j++){
			floorMap[y+i][x+j] = block_type;
		}
	}
}

void FloorMap::fillBlock(Room room, int block_type){
	fillBlock(room.x,room.y,room.w,room.h,block_type);
}

void FloorMap::makePath(int sx, int sy, int ex, int ey){
	if(getRand(0,1)){
		if(sx > ex){
			swap(sx,ex);
			swap(sy,ey);
		}
		int tt = 1;
		fillBlock(sx,sy,ex-sx+1,tt);
		fillBlock(ex,min(ey,sy),tt,max(ey,sy)-min(ey,sy));
	}
	else{
		if(sy > ey){
			swap(sx,ex);
			swap(sy,ey);
		}
		int tt = 1;
		fillBlock(sx,sy,tt,ey-sy+1);
		fillBlock(min(ex,sx),ey,max(ex,sx)-min(ex,sx),tt);
	}
}

bool FloorMap::emptyNeighbour(int x, int y){
	if( floorMap[x][y] == EMPTYOUT && floorMap[x-1][y] == EMPTYOUT &&
			floorMap[x+1][y] == EMPTYOUT && floorMap[x][y+1] == EMPTYOUT &&
			floorMap[x][y-1] == EMPTYOUT){
		floorMap[x][y] = CHESTOUT;
		return true;
	}
	return false;
}

void FloorMap::convert(int px, int py){
	switch(floorMap[px][py]){
		case EMPTYOUT:
			floorMap[px][py] = EMPTYSEEN;
			break;
		case CHESTOUT:
			if(getRand(1,100)<BLUECHESTCHANCE)
				floorMap[px][py] = CHEST2CLOSED;
			else
				floorMap[px][py] = CHESTCLOSED;
			break;
		case WALLEDGEOUT:
			floorMap[px][py] = WALLEDGE;
			break;
		case STAIROUT:
			floorMap[px][py] = STAIRSEEN;
			break;
	}
}

void FloorMap::setBorders(){
	for(int i = 1 ; i < mHeight-1 ; i++){
		for(int j = 1 ; j < mWidth-1 ; j++){
			if(floorMap[i][j] == WALL && (floorMap[i-1][j]==EMPTYOUT||floorMap[i+1][j]==EMPTYOUT||floorMap[i][j+1]==EMPTYOUT||floorMap[i][j-1]==EMPTYOUT)){
				floorMap[i][j] = WALLEDGEOUT;
			}
		}
	}
}

bool FloorMap::joinRoom(int a, int b){
	if(a==b){
		return false;
	}
	int startx = roomList[a].x + getRand(2,roomList[a].w-2),
		starty = roomList[a].y + getRand(2,roomList[a].h-2),
		endx = roomList[b].x + getRand(2,roomList[b].w-2),
		endy = roomList[b].y + getRand(2,roomList[b].h-2);
	makePath(startx,starty,endx,endy);
	return true;
}

void FloorMap::checkEnemy(int x,int y){
	for(int i = 0; i < enemyList.size(); i++){
		if(enemyList[i].x == x && enemyList[i].y == y){
			enemyList[i].setVisible();
		}
	}
}

void FloorMap::setEnemyInvisible(){
	for(int i = 0; i < enemyList.size(); i++){
		enemyList[i].setInvisible();
	}
}

int FloorMap::getEnemyLevel(){
	return getRand(1,2);
}

int FloorMap::getEnemyType(){
	return getRand(0,1);
}

void FloorMap::addEnemies(){
	for(int i = 0; i < MAX_ENEMY_NO - getRand(0,2); ){
		int rno = getRand(0,troom-1),tx,ty;
		bool flag = true;
		while(flag){
			tx = getRand(roomList[rno].y+1,roomList[rno].y+roomList[rno].h-1);
			ty = getRand(roomList[rno].x+1,roomList[rno].x+roomList[rno].w-1);
			if(floorMap[tx][ty] == EMPTYOUT){
				flag = false;
			}
		}
		floorMap[tx][ty] = WALL;
		Enemy te;
		te.init(getRand(0,2),max(1,floorNo-1+getRand(0,1)));
		te.x = tx, te.y = ty;
		enemyList.push_back(te);
		i++;
	}
	for(int i = 0; i < enemyList.size() ; i++){
		floorMap[enemyList[i].x][enemyList[i].y] = EMPTYOUT;
	}
}

void FloorMap::moveEnemies(Logger &log, Player &player){
	if(currentEnemyProcess==0){
		for(int i = 0; i < enemyList.size(); i++){
			enemyList[i].moved = false;
		}
	}
	currentEnemyProcess = 1;
	for(int i = 0; i < enemyList.size(); i++){
		if(enemyList[i].moved)
			continue;
		if(isAdj(enemyList[i].x,enemyList[i].y,player.x,player.y)){
			int temp = damageCalc(player,enemyList[i]);
			if(temp)
				if(temp < 0){
					temp = -temp;
					log.logCrit(enemyList[i].name,player.pName,temp);
				}
				else
					log.logDamage(enemyList[i].name,player.pName,temp);
			else
				log.logMiss(enemyList[i].name);
			player.getHit(temp);
			enemyList[i].moved = true;
			return;
		}
		else if(enemyList[i].visible){
			int dir = getDisttoPlayer(enemyList[i]);
			switch(dir){
				case UP:
				enemyList[i].x--;enemyList[i].moved = true;break;
				case DOWN:
				enemyList[i].x++;enemyList[i].moved = true;break;
				case LEFT:
				enemyList[i].y--;enemyList[i].moved = true;break;
				case RIGHT:
				enemyList[i].y++;enemyList[i].moved = true;break;
			}
		}
	}
	for(int i = 0; i < enemyList.size(); i++){
		if(enemyList[i].moved)
			continue;
		else if(enemyList[i].visible){
			int dir = getDisttoPlayer(enemyList[i]);
			switch(dir){
				case UP:
				enemyList[i].x--;enemyList[i].moved = true;break;
				case DOWN:
				enemyList[i].x++;enemyList[i].moved = true;break;
				case LEFT:
				enemyList[i].y--;enemyList[i].moved = true;break;
				case RIGHT:
				enemyList[i].y++;enemyList[i].moved = true;break;
			}
		}
	}
	currentEnemyProcess = -1;
	//printf("Moved Enemies\n");
}


bool FloorMap::isFree(int x, int y){
	if(isWalkable(x,y)){
		if(x==playerPos.x && y==playerPos.y)
			return false;
		for(int i = 0; i < enemyList.size() ; i++){
			if(x == enemyList[i].x && y == enemyList[i].y){
				return false;
			}
		}
		return true;
	}
	return false;
}

bool FloorMap::isWalkable(int x, int y){
	switch(floorMap[x][y]){
		case EMPTYOUT:case EMPTYSEEN:case STAIROUT:case STAIRSEEN:
			return true;
		default:
			return false;
	}
}

int FloorMap::isEnemy(int x, int y){
	if(isWalkable(x,y)){
		for(int i = 0; i < enemyList.size() ; i++){
			if(x == enemyList[i].x && y == enemyList[i].y){
				return i+1;
			}
		}
	}
	return 0;
}

int FloorMap::damageCalc(Actor &a, Actor &b){
	float t = b.atk/a.def;
	t *= b.level;
	if(getRand(0,100) < b.acc){
		if(getRand(1,15) <= b.luck){
			t *= -2.2;
			if (t < -1)
				return (int)t;
			else
				return -1;
		}
		return max((int)t,1);
	}
	else
		return 0;
}


void FloorMap::attackEnemy(Player &player, Logger &log, int eno){
	eno-=1;
	int t = damageCalc(enemyList[eno],player);
	if(t){
		if(t < 0){
			t = -t;
			log.logCrit(player.pName,enemyList[eno].name,t);
		}
		else{
			log.logDamage(player.pName,enemyList[eno].name,t);
		}
	}
	else
		log.logMiss(player.pName);
	if(enemyList[eno].getHit(t)){
		log.logKilled(enemyList[eno].name);
		int xp = getXP(enemyList[eno]);
		log.logXP(xp);
		if(player.gainXP(xp)){
			log.logLevelUp(player.level);
		}
		enemyList.erase(enemyList.begin()+eno);
	}
}

int FloorMap::getDisttoPlayer(Enemy &e){
	float d[5];
	int playerX = playerPos.x, playerY = playerPos.y;
	d[0] = isFree(e.x-1,e.y)? dist(e.x-1,e.y,playerX,playerY) : MAX_DIST;
	d[1] = isFree(e.x+1,e.y)? dist(e.x+1,e.y,playerX,playerY) : MAX_DIST;
	d[2] = isFree(e.x,e.y-1)? dist(e.x,e.y-1,playerX,playerY) : MAX_DIST;
	d[3] = isFree(e.x,e.y+1)? dist(e.x,e.y+1,playerX,playerY) : MAX_DIST;
	d[4] = dist(e.x,e.y,playerX,playerY) + 0.8;
	return min(d,5);
}

int FloorMap::getXP(Enemy &enemy){
	float t = 1.5;
	t = t*enemy.level*((enemy.type+3)/3.0);
	return t;
}

void FloorMap::putStairs(){
	int rno = getRand(0,troom-1);
	bool flag = true;
	int tx = roomList[rno].y+roomList[rno].h/2;
	int ty = roomList[rno].x+roomList[rno].w/2;
	while(flag){
		tx = getRand(roomList[rno].y+1,roomList[rno].y+roomList[rno].h-1);
		ty = getRand(roomList[rno].x+1,roomList[rno].x+roomList[rno].w-1);
		if(isFree(tx,ty)){
			flag = false;
		}
	}
	floorMap[tx][ty] = STAIROUT;
}

void FloorMap::goToNextLevel(Logger &log, Graphics &graphics){
	log.clearLog();
	this->setFloor(log,this->floorNo+1);
	this->genMap(graphics);
}
