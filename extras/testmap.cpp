#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <queue>
#include <sstream>
#include <iostream>
#include "LTexture.h"
#include "FloorMap.h"
#include "LTimer.h"
#include "LText.h"
#include "Logger.h"

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* gFont = NULL;
TTF_Font* logFont = NULL;
LTexture tiles;
LTexture enemy;
const int enemyNo = 2;
SDL_Rect enemyClip[2];
const int ClipNo = 10;
SDL_Rect tileClip[ClipNo];
SDL_Rect PlayerClip;
const int textno = 9;
LText staticText[textno];
LText dynamicText;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 820;
const SDL_Rect GAMEVIEWPORT = {0,0,990,660};
const SDL_Rect INVENTORYVIEWPORT = {990,220,210,600};
const SDL_Rect LOGVIEWPORT = {0,660,990,160};
const SDL_Rect STATUSVIEWPORT = {990,0,210,220};
const SDL_Rect FULLVIEWPORT = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
const int TILE_SIZE = 15;
const int FPS = 120;
const int SCREEN_TICKS_PER_FRAME = 1000/FPS;
const SDL_Color StatColor = {0x0,0x0,0x0,0xFF};
const SDL_Color StatColor2 = {0x77,0x0,0x0,0xFF};
const SDL_Color logColor = {0xFF,0xFF,0xFF,0xFF};
enum Screen{
	SCREENMENU = 0,
	SCREENGAME = 1,
	SCREENOVER = 2,
};

bool init(){
	bool success = true;
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		window = SDL_CreateWindow( "MAP GENERATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		
		if(window == NULL){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
				
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

			}
		}
	}
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	gFont = TTF_OpenFont( "resources/VT323-Regular.ttf", 24 );
	if( gFont == NULL ){
		printf( "Failed to load game font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	logFont = TTF_OpenFont( "resources/Raleway-Medium.ttf", 12 );
	if( logFont == NULL ){
		printf( "Failed to load log font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	if( !tiles.loadFromFile(renderer, "resources/blocksprite2.png") ){
		printf( "Failed to load block texture!\n" );
		success = false;
	}
	if( !enemy.loadFromFile(renderer, "resources/enemysprite.png") ){
		printf( "Failed to load enemy texture!\n" );
		success = false;
	}
	PlayerClip.x = 300, PlayerClip.y = 0, PlayerClip.w = 100, PlayerClip.h = 100;
	tileClip[0].x = 0, tileClip[0].y = 0, tileClip[0].w = 100, tileClip[0].h = 100;
	tileClip[2].x = 100, tileClip[2].y = 0, tileClip[2].w = 100, tileClip[2].h = 100;
	tileClip[1] = tileClip[0];
	tileClip[3] = tileClip[0];
	tileClip[4].x = 200, tileClip[4].y = 0, tileClip[4].w = 100, tileClip[4].h = 100;
	tileClip[5].x = 400, tileClip[5].y = 0, tileClip[5].w = 100, tileClip[5].h = 100;
	tileClip[6].x = 500, tileClip[6].y = 0, tileClip[6].w = 100, tileClip[6].h = 100;
	tileClip[7] = tileClip[0];
	tileClip[8] = tileClip[0];
	tileClip[9].x = 0, tileClip[9].y = 100, tileClip[9].w = 100, tileClip[9].h = 100;
	staticText[0].loadFromRenderedText(renderer,gFont,"PLAYER:",StatColor);
	staticText[1].loadFromRenderedText(renderer,gFont,"LEVEL:",StatColor);
	staticText[2].loadFromRenderedText(renderer,gFont,"HEALTH:",StatColor);
	staticText[3].loadFromRenderedText(renderer,gFont,"ATTACK:",StatColor);
	staticText[4].loadFromRenderedText(renderer,gFont,"DEFENSE:",StatColor);
	staticText[5].loadFromRenderedText(renderer,gFont,"XP:",StatColor);
	staticText[6].loadFromRenderedText(renderer,gFont,"NEXTLVL:",StatColor);
	staticText[7].loadFromRenderedText(renderer,gFont,"STATUS",StatColor);
	staticText[8].loadFromRenderedText(renderer,gFont,"INVENTORY",StatColor);
	enemyClip[0].x = 0,enemyClip[0].y = 0,enemyClip[0].w = 100,enemyClip[0].h = 100;
	enemyClip[1].x = 100,enemyClip[1].y = 0,enemyClip[1].w = 100,enemyClip[1].h = 100;
	return success;
}

void close()
{
	//Free loaded images
	tiles.free();
	for(int i = 0; i < textno; i++){
		staticText[i].free();
	}
	dynamicText.free();
	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	TTF_CloseFont(gFont);
	TTF_CloseFont(logFont);
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void renderViewCone(FloorMap fmap){
	int playerX=fmap.getPlayerX(),playerY=fmap.getPlayerY(),VIEWSIZE=fmap.VIEWSIZE;
	int clip,mWidth = fmap.getWidth(),mHeight = fmap.getHeight();
	std::queue<int> q1,q2,q3;
	q1.push(playerX);q2.push(playerY);q3.push(VIEWSIZE);
	bool visited[mHeight][mWidth];
	for(int i = 0; i < mHeight ; i++){
		for(int j = 0; j < mWidth ; j++){
			visited[i][j] = false;
		}
	}
	visited[playerX][playerY] = true;
	while(!q1.empty()){
		int t1 = q1.front(),t2 = q2.front(), t3 = q3.front();
		q1.pop();q2.pop();q3.pop();
		if(t3 && !visited[t1-1][t2] && fmap.getBlock(t1-1,t2)!=WALL && fmap.getBlock(t1-1,t2)!=WALLEDGE){
			q1.push(t1-1);q2.push(t2);q3.push(t3-1);visited[t1-1][t2] = true;
		}
		if(t3 && !visited[t1+1][t2] && fmap.getBlock(t1+1,t2)!=WALL && fmap.getBlock(t1+1,t2)!=WALLEDGE){
			q1.push(t1+1);q2.push(t2);q3.push(t3-1);visited[t1+1][t2] = true;
		}
		if(t3 && !visited[t1][t2+1] && fmap.getBlock(t1,t2+1)!=WALL && fmap.getBlock(t1,t2+1)!=WALLEDGE){
			q1.push(t1);q2.push(t2+1);q3.push(t3-1);visited[t1][t2+1] = true;
		}
		if(t3 && !visited[t1][t2-1] && fmap.getBlock(t1,t2-1)!=WALL && fmap.getBlock(t1,t2-1)!=WALLEDGE){
			q1.push(t1);q2.push(t2-1);q3.push(t3-1);visited[t1][t2-1] = true;
		}
		switch(fmap.getBlock(t1,t2)){
			case EMPTYSEEN:clip=EMPTYBRIGHT;break;
			default:clip = fmap.getBlock(t1,t2);
		}
		tiles.render(renderer, (t2)*TILE_SIZE, (t1)*TILE_SIZE,TILE_SIZE, TILE_SIZE, &tileClip[clip]);
	}
}

void renderBoard(FloorMap fmap){
	for(int i = 0; i < fmap.getHeight() ; i++){
		for(int j = 0; j < fmap.getWidth() ; j++){
			int tile = fmap.getBlock(i,j);
			tiles.render(renderer, j*TILE_SIZE, i*TILE_SIZE,TILE_SIZE, TILE_SIZE, &tileClip[tile]);
		}
	}

	renderViewCone(fmap);
}

void renderPlayer(FloorMap fmap){
	tiles.render(renderer,fmap.getPlayerY()*TILE_SIZE, fmap.getPlayerX()*TILE_SIZE, TILE_SIZE, TILE_SIZE, &PlayerClip);
	SDL_RenderPresent(renderer);
}

void renderEnemies(FloorMap fmap){
	for(int i = 0; i < fmap.enemyList.size(); i++){
		if(fmap.enemyList[i].visible){
			enemy.render(renderer,fmap.enemyList[i].y*TILE_SIZE, fmap.enemyList[i].x*TILE_SIZE, TILE_SIZE, TILE_SIZE, &enemyClip[fmap.enemyList[i].type]);
		}
	}
}

void renderInventory(FloorMap fmap){
	SDL_SetRenderDrawColor(renderer,0xAA,0x66,0x30,0xFF);
	SDL_Rect d = {0,0,INVENTORYVIEWPORT.w,INVENTORYVIEWPORT.h};
	SDL_RenderFillRect(renderer,&d);
	staticText[8].renderCenterX(renderer,INVENTORYVIEWPORT.w,5);
}

void renderStat(int x,int y){
	stringstream st1;
	st1.str("");
	st1<<x;
	dynamicText.loadFromRenderedText(renderer,gFont,st1.str().c_str(),StatColor2);
	dynamicText.renderCenterX(renderer,STATUSVIEWPORT.w,y,STATUSVIEWPORT.w/4);
}

void renderStat(string x,int y){
	dynamicText.loadFromRenderedText(renderer,gFont,x.c_str(),StatColor2);
	dynamicText.renderCenterX(renderer,STATUSVIEWPORT.w,y,STATUSVIEWPORT.w/4);
}

void renderLog(FloorMap fmap){
	SDL_SetRenderDrawColor(renderer,0x0,0x0,0x0,0xFF);
	SDL_Rect d = {0,0,LOGVIEWPORT.w,LOGVIEWPORT.h};
	SDL_RenderFillRect(renderer,&d);
	for(int i = 0; i < fmap.log.logMessages.size(); i++){
		dynamicText.loadFromRenderedText(renderer, logFont, fmap.log.logMessages[i].c_str(),logColor);
		dynamicText.render(renderer,5,LOGVIEWPORT.h+15*(i-fmap.log.logMessages.size()));
	}
}

void renderStatus(FloorMap fmap){
	SDL_SetRenderDrawColor(renderer,0xBB,0xBB,0xBB,0xFF);
	SDL_Rect d = {0,0,STATUSVIEWPORT.w,STATUSVIEWPORT.h};
	SDL_RenderFillRect(renderer,&d);
	SDL_SetRenderDrawColor(renderer,0x33,0x33,0x88,0xFF);
	d = {0,55*textno+20,STATUSVIEWPORT.w,STATUSVIEWPORT.h-55*textno-20};
	SDL_RenderFillRect(renderer,&d);
	for(int i = 0; i < 7 ; i++){
		staticText[i].renderCenterX(renderer,STATUSVIEWPORT.w/2,25*i);
	}
	renderStat(fmap.player.pName,0);
	renderStat(fmap.player.level,25);
	renderStat(fmap.player.getHealthStr(),50);
	renderStat(fmap.player.atk,75);
	renderStat(fmap.player.def,100);
	renderStat(fmap.player.xp,125);
	renderStat(fmap.player.nextxp,150);
	staticText[7].renderCenterX(renderer,STATUSVIEWPORT.w,190);
}

void renderAll(FloorMap fmap, int screen){
	if(screen == SCREENMENU){
		SDL_RenderSetViewport( renderer, &FULLVIEWPORT);
		SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
	if(screen == SCREENGAME){	
		SDL_RenderSetViewport( renderer, &GAMEVIEWPORT);
		renderBoard(fmap);
		renderEnemies(fmap);
		renderPlayer(fmap);
		SDL_RenderSetViewport( renderer, &INVENTORYVIEWPORT);
		renderInventory(fmap);
		SDL_RenderSetViewport( renderer, &STATUSVIEWPORT);
		renderStatus(fmap);
		SDL_RenderSetViewport( renderer, &LOGVIEWPORT);
		renderLog(fmap);
		SDL_RenderPresent(renderer);
	}
}

int main( int argc, char* args[] ){
	if(!init()){
		printf("Failed to initialize\n");
		close();
		return 0;
	}
	else if(!loadMedia()){
		printf("Failed to load media\n");
		close();
		return 0;
	}
	SDL_Event e;
	bool quit = false;
	int rooms = 10,enemies = 10;
	LTimer frameTimer;
	int screen = SCREENGAME;
	FloorMap fmap("ROHITH");
	fmap.genMap(rooms,enemies);
	renderAll(fmap,screen);
	int moveframe = -1;
	
	//TODO: Move Event Handling into respective classes
	
	while(!quit){
		frameTimer.start();
		while(SDL_PollEvent( &e ) != 0){
			if( e.type == SDL_QUIT ){
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_r){
				fmap.genMap(rooms);
				renderAll(fmap,screen);
			}
			else{
				if(screen == SCREENGAME){
					fmap.handleEvent(e);
				}
			}
		}
		if(fmap.frameDone()){
			quit = true;
		}
		if(fmap.newData){
			renderAll(fmap,screen);
			fmap.renderDone();
		}
		
		int frameTicks = frameTimer.getTicks();
		if( frameTicks < SCREEN_TICKS_PER_FRAME ){
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}
	}
	fmap.free();
	close();
	return 0;
}
