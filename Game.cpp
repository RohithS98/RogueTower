#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Game.h"
//#include "Actors.h"
//#include "Utils.h"

//Initializes SDL and starts game loop
Game::Game(){
	#ifdef DEBUG
	std::cout<<"Initializing SDL"<<std::endl;
	#endif
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init( IMG_INIT_PNG ) ;
	TTF_Init();
	gameLoop();
}

//Quits SDL and subsystems
Game::~Game(){
	#ifdef DEBUG
	std::cout<<"Quiting SDL"<<std::endl;
	#endif
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//Game loop. Handles all events and drawing and updating of elements
void Game::gameLoop(){
	#ifdef DEBUG
	std::cout<<"Starting Game Loop"<<std::endl;
	#endif
	Graphics graphics;
	SDL_Event e;

	log.clearLog();
	fmap = FloorMap(graphics);
	fmap.setFloor(log,1);
	fmap.genMap(graphics);
	Vector2 playPos = fmap.putPlayer();
	player.init(graphics);
	player.setPosition(playPos);
	//fmap.updateView();
	currentKey = 0;
	currentFrame = -1;

	float LAST_UPDATE;
	while(true){
		LAST_UPDATE = SDL_GetTicks();
		graphics.clear();
		while(SDL_PollEvent(&e) != 0){	//Handle all inputs
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

		this->update(currentKey,currentFrame,graphics);	//Update frame
		if(currentFrame >= 0){					//Increment frame counter
			currentFrame = (currentFrame+1)%FRAME_PER_MOVE;
		}
		if( fmap.currentEnemyProcess == 1 ){	//If enemies were not processed fully
			currentKey = 0;						//Stop movement
			currentFrame = -1;
		}

		draw(graphics);		//Draw graphics

		float CURRENT_TIME = SDL_GetTicks();
		if(CURRENT_TIME - LAST_UPDATE < FRAME_TIME){	//Wait to maintain frame rate
			SDL_Delay(FRAME_TIME - CURRENT_TIME + LAST_UPDATE);
		}
	}
}

//Draws all the graphics
void Game::draw(Graphics &graphics){
	graphics.defaultViewPort();
	graphics.clear();
	graphics.setViewPort(GAME_VIEWPORT);
	fmap.drawMap(graphics);
	fmap.drawViewCone(graphics);
	fmap.drawItems(graphics);
	player.draw(graphics);
	fmap.drawEnemy(graphics);
	graphics.setViewPort(LOG_VIEWPORT);
	log.render(graphics);
	graphics.setViewPort(STAT_VIEWPORT);
	player.renderStat(graphics);
	graphics.flip();
}

//Updates map and player
void Game::update(SDL_Keycode key, int currentFrame, Graphics &graphics){
	fmap.handleMove(player,log,key,currentFrame,graphics);
	player.setPosition(fmap.getPlayerPos());
}
