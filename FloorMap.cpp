#include "FloorMap.h"
#include "Utils.h"

FloorMap::FloorMap(string pname,int t){
	int seed = 1530335882;
	//int seed = time(NULL);
	printf("Seed:%d\n",seed);
	srand(seed);
	MIN_ROOM_SIZE = 7;
	MAX_ROOM_SIZE = 10;
	VIEWSIZE = 10;
	roomList = NULL;
	newData = false;
	currentKey = 0;
	nextMoveFrame = -1;
	player.init(t,pname);
	floorNo = 1;
	currentEnemyProcess = -1;
}

void FloorMap::fillBlock(int x, int y, int width, int height,int block_type){
	for(int i = 0 ; i < height ; i++){
		for(int j = 0; j < width; j++){
			floorMap[y+i][x+j] = block_type;
		}
	}
}

void FloorMap::putPlayer(){
	int rno = getRand(0,troom-1);
	bool flag = true;
	int tx = roomList[rno].y+roomList[rno].h/2;
	int ty = roomList[rno].x+roomList[rno].w/2;
	while(flag){
		tx = getRand(roomList[rno].y+1,roomList[rno].y+roomList[rno].h-1);
		ty = getRand(roomList[rno].x+1,roomList[rno].x+roomList[rno].w-1);
		if(floorMap[tx][ty] == EMPTYOUT){
			playerX = tx;
			playerY = ty;
			flag = false;
		}
	}
	log.clearLog();
	log.logNewFloor(floorNo);
}

int FloorMap::getPlayerX(){
	return playerX;
}

int FloorMap::getPlayerY(){
	return playerY;
}

void FloorMap::attackEnemy(int eno){
	currentKey = 0;
	eno-=1;
	log.logAttack(player.pName,enemyList[eno].name);
	int t = damageCalc(enemyList[eno],player);
	if(t)
		log.logDamage(player.pName,enemyList[eno].name,t);
	else
		log.logMiss(player.pName);
	if(enemyList[eno].getHit(t)){
		log.logKilled(enemyList[eno].name);
		enemyList.erase(enemyList.begin()+eno);
	}
	log.logLine();
}

bool FloorMap::movePlayer(int direction){
	bool ret = false;
	int t = -1;
	switch(direction){
		case UP:
			if(isFree(playerX-1,playerY)){
			playerX--;ret=true;
			}
			else if(t = isEnemy(playerX-1,playerY)){
				ret=true;
				attackEnemy(t);
			}
			break;
		case LEFT:
			if(isFree(playerX,playerY-1)){
			playerY--;ret=true;
			}
			else if(t = isEnemy(playerX,playerY-1)){
				ret=true;
				attackEnemy(t);
			}
			break;
		case DOWN:
			if(isFree(playerX+1,playerY)){
			playerX++;ret=true;
			}
			else if(t = isEnemy(playerX+1,playerY)){
				ret=true;
				attackEnemy(t);
			}
			break;
		case RIGHT:
			if(isFree(playerX,playerY+1)){
			playerY++;ret=true;
			}
			else if(t = isEnemy(playerX,playerY+1)){
				ret=true;
				attackEnemy(t);
			}
			break;
	}
	updateView();
	return ret;
}

void FloorMap::convert(int px, int py){
	switch(floorMap[px][py]){
		case EMPTYOUT:
			floorMap[px][py] = EMPTYSEEN;
			break;
		case CHESTOUT:
			floorMap[px][py] = CHESTCLOSED;
			break;
		case WALLEDGEOUT:
			floorMap[px][py] = WALLEDGE;
			break;
	}
}

bool FloorMap::inbounds(int px, int py){
	if(py < mWidth && px >=0 && px < mHeight && py >=0){
		return true;
	}
	return false;
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

void FloorMap::updateView(){
	std::queue<int> q1,q2,q3;
	q1.push(playerX);q2.push(playerY);q3.push(VIEWSIZE);
	bool visited[mHeight][mWidth];
	for(int i = 0; i < mHeight ; i++){
		for(int j = 0; j < mWidth ; j++){
			visited[i][j] = false;
		}
	}
	setEnemyInvisible();
	visited[playerX][playerY] = true;
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

bool Room::isIntersecting(Room room){
	if (x >= room.x + room.w+5 || w + x <= room.x-5)
        return false;
 
     if (y >= room.y + room.h+5 || y + h <= room.y-5)
        return false;
 
    return true;
}

void FloorMap::fillBlock(Room room, int block_type){
	fillBlock(room.x,room.y,room.w,room.h,block_type);
}

int FloorMap::getBlock(int x, int y){
	return floorMap[x][y];
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
	if( floorMap[x][y] == EMPTYOUT && floorMap[x-1][y] == EMPTYOUT && floorMap[x+1][y] == EMPTYOUT && floorMap[x][y+1] == EMPTYOUT && floorMap[x][y-1] == EMPTYOUT){
		floorMap[x][y] = CHESTOUT;
		return true;
	}
	return false;
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
	int startx = roomList[a].x + getRand(3,roomList[a].w-3),
		starty = roomList[a].y + getRand(3,roomList[a].h-3),
		endx = roomList[b].x + getRand(3,roomList[b].w-3),
		endy = roomList[b].y + getRand(3,roomList[b].h-3);
	makePath(startx,starty,endx,endy);
	return true;
}

void FloorMap::addEnemies(int e){
	for(int i = 0; i < e ; ){
		int rno = getRand(0,troom-1),tx,ty;
		bool flag = true;
		while(flag){
			tx = getRand(roomList[rno].y+1,roomList[rno].y+roomList[rno].h-1);
			ty = getRand(roomList[rno].x+1,roomList[rno].x+roomList[rno].w-1);
			if(floorMap[tx][ty] == EMPTYOUT && (playerX!=tx || playerY!=ty)){
				flag = false;
			}
		}
		floorMap[tx][ty] = WALL;
		Enemy te;
		te.init(getRand(0,1),1);
		te.x = tx, te.y = ty;
		enemyList.push_back(te);
		i++;
	}
	for(int i = 0; i < enemyList.size() ; i++){
		floorMap[enemyList[i].x][enemyList[i].y] = EMPTYOUT;
	}
}

void FloorMap::genMap(int rooms, int enemies, int chestNo){
	free();
	mWidth = WIDTH;
	mHeight = HEIGHT;
	fillBlock(0,0,mWidth,mHeight,WALL);
	roomList = new Room[rooms];
	bool flag;
	troom = 0;
	int k = 0;
	
	for(int i = 0; i < rooms && k < 1000;){
		Room tempRoom;
		tempRoom.w = getRand(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
		tempRoom.h = getRand(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
		tempRoom.x = getRand(3, mWidth - tempRoom.w - 3);
		tempRoom.y = getRand(3, mHeight - tempRoom.h - 3);
		flag = true;
		k += 1;
		for(int j = 0; j < i; j++){
			if(tempRoom.isIntersecting(roomList[j])){
				flag = false;
				break;
			}
		}
		if(flag){
			roomList[i] = tempRoom;
			i++;
			troom++;
			k = 0;
		}
	}
	
	for(int i = 0; i < troom; i++){
		fillBlock(roomList[i],EMPTYOUT);
	}
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
	putPlayer();
	addEnemies(enemies);
	updateView();
	newData = true;
}

int FloorMap::getWidth(){
	return mWidth;
}

int FloorMap::getHeight(){
	return mHeight;
}

void FloorMap::handleEvent(SDL_Event e){
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_w:case SDLK_a:case SDLK_s:case SDLK_d:case SDLK_LEFT:case SDLK_RIGHT:case SDLK_UP:case SDLK_DOWN:
				currentKey = e.key.keysym.sym;
				nextMoveFrame = 0;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_w:case SDLK_a:case SDLK_s:case SDLK_d:case SDLK_LEFT:case SDLK_RIGHT:case SDLK_UP:case SDLK_DOWN:
				if(currentKey == e.key.keysym.sym){
					currentKey = 0;
					nextMoveFrame = -1;
				}
		}
	}
}

bool FloorMap::handleMove(){
	bool ret = false;
	switch(currentKey){
		case SDLK_w:
		case SDLK_UP:
			ret = movePlayer(UP);
			break;
		case SDLK_a:
		case SDLK_LEFT:
			ret = movePlayer(LEFT);
			break;
		case SDLK_s:
		case SDLK_DOWN:
			ret = movePlayer(DOWN);
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			ret = movePlayer(RIGHT);
			break;
	}
}

int FloorMap::getDisttoPlayer(Enemy e){
	float d[5];
	d[0] = isFree(e.x-1,e.y)? dist(e.x-1,e.y,playerX,playerY) : MAX_DIST;
	d[1] = isFree(e.x+1,e.y)? dist(e.x+1,e.y,playerX,playerY) : MAX_DIST;
	d[2] = isFree(e.x,e.y-1)? dist(e.x,e.y-1,playerX,playerY) : MAX_DIST;
	d[3] = isFree(e.x,e.y+1)? dist(e.x,e.y+1,playerX,playerY) : MAX_DIST;
	d[4] = dist(e.x,e.y,playerX,playerY);
	return min(d,5);
}

bool FloorMap::moveEnemies(){
	if(currentEnemyProcess==0){
		for(int i = 0; i < enemyList.size(); i++){
			enemyList[i].moved = false;
		}
	}
	currentEnemyProcess = 1;
	for(int i = 0; i < enemyList.size(); i++){
		if(enemyList[i].moved)
			continue;
		if(isAdj(enemyList[i].x,enemyList[i].y,playerX,playerY)){
			log.logAttack(enemyList[i].name,player.pName);
			int temp = damageCalc(player,enemyList[i]);
			if(temp)
				log.logDamage(enemyList[i].name,player.pName,temp);
			else
				log.logMiss(enemyList[i].name);
			player.getHit(temp);
			currentKey = 0;
			enemyList[i].moved = true;
			log.logLine();
			if(haltFor(ATTACKDELAY))
				return true;
			else
				return false;
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
	return false;
}

bool FloorMap::frameDone(){
	if(nextMoveFrame!=-1){
		if(!nextMoveFrame && currentEnemyProcess == -1){
			bool t = handleMove();
			if(t){
				currentEnemyProcess = 0;
			}
			newData = true;
		}
		nextMoveFrame = (nextMoveFrame+1)%FRAME_PER_MOVE;
	}
	if(currentEnemyProcess!=-1){
		newData = true;
		nextMoveFrame = (nextMoveFrame+1)%FRAME_PER_MOVE;
		return moveEnemies();
	}
	return false;
}

void FloorMap::free(){
	if(roomList!=NULL)
		delete[] roomList;
	enemyList.erase(enemyList.begin(),enemyList.end());
}

void FloorMap::renderDone(){
	newData = false;
}

bool FloorMap::isFree(int x, int y){
	if(floorMap[x][y] == EMPTYOUT || floorMap[x][y] == EMPTYSEEN){
		if(x==playerX&&y==playerY)
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

int FloorMap::isEnemy(int x, int y){
	if(floorMap[x][y] == EMPTYOUT || floorMap[x][y] == EMPTYSEEN){
		for(int i = 0; i < enemyList.size() ; i++){
			if(x == enemyList[i].x && y == enemyList[i].y){
				return i+1;
			}
		}
	}
	return 0;
}

int FloorMap::damageCalc(Actor a, Actor b){
	float t = b.atk/a.def;
	t *= b.level;
	if(getRand(0,100) < b.acc)
		return max((int)t,1);
	else
		return 0;
}

bool FloorMap::haltFor(int time){
	LTimer stopTimer;
	SDL_Event e;
	stopTimer.start();
	while(stopTimer.getTicks() < time){
		while(SDL_PollEvent( &e ) != 0){
			if( e.type == SDL_QUIT ){
				return true;
			}
		}
	}
	return false;
}
