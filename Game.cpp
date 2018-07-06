#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "Actors.h"
#include "Utils.h"
#include "Logger.h"

Game::Game(){
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init( IMG_INIT_PNG ) ;
	TTF_Init();
	gameLoop();
}

Game::~Game(){
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::gameLoop(){
	Graphics graphics;
	SDL_Event e;
	
	log.clearLog();
	fmap = FloorMap(graphics);
	fmap.setFloor(log,1);
	fmap.genMap(graphics);
	Vector2 playPos = fmap.putPlayer();
	player.init(graphics);
	player.setPosition(playPos);
	fmap.updateView();
	currentKey = 0;
	currentFrame = -1;
	
	float LAST_UPDATE;
	while(true){
		LAST_UPDATE = SDL_GetTicks();
		graphics.clear();
		while(SDL_PollEvent(&e) != 0){
			if( e.type == SDL_QUIT ){
				return ;
			}
			if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
				switch(e.key.keysym.sym){
					case SDLK_w:case SDLK_a:case SDLK_s:case SDLK_d:case SDLK_UP:
					case SDLK_LEFT:case SDLK_DOWN:case SDLK_RIGHT:
						currentKey = e.key.keysym.sym;
						currentFrame = 0;
				}
			}
			else if( e.type == SDL_KEYUP ){
				if(currentKey == e.key.keysym.sym){
					currentKey = 0;
					currentFrame = -1;
				}
			}
		}
		
		this->update(currentKey,currentFrame);
		if(currentFrame >= 0){
			currentFrame = (currentFrame+1)%FRAME_PER_MOVE;
		}
		if( fmap.currentEnemyProcess == 1 ){
			printf("T");
			currentKey = 0;
			currentFrame = -1;
		}
		
		draw(graphics);
		
		float CURRENT_TIME = SDL_GetTicks();
		if(CURRENT_TIME - LAST_UPDATE < FRAME_TIME){
			SDL_Delay(FRAME_TIME - CURRENT_TIME + LAST_UPDATE);
		}
	}
}

void Game::draw(Graphics &graphics){
	graphics.setViewPort(GAME_VIEWPORT);
	fmap.drawMap(graphics);
	fmap.drawViewCone(graphics);
	fmap.drawEnemy(graphics);
	player.draw(graphics);
	graphics.setViewPort(LOG_VIEWPORT);
	log.render(graphics);
	//graphics.setViewPort(STAT_VIEWPORT);
	//player.renderStats(graphics);
	graphics.flip();
}

void Game::update(SDL_Keycode key, int currentFrame){
	fmap.handleMove(player,log,key,currentFrame);
	player.setPosition(fmap.getPlayerPos());
}
