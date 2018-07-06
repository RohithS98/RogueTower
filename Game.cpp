#include <SDL2/SDL.h>
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
	
	fmap = FloorMap(graphics);
	fmap.setFloor(1);
	fmap.genMap(MAX_ROOMS);
	Vector2 playPos = fmap.putPlayer();
	int x = 0, y = 20;
	
	float LAST_UPDATE;
	while(true){
		LAST_UPDATE = SDL_GetTicks();
		graphics.clear();
		while(SDL_PollEvent(&e) != 0){
			if( e.type == SDL_QUIT ){
				return ;
			}
		}
		fmap.render(graphics);
		graphics.flip();
		float CURRENT_TIME = SDL_GetTicks();
		if(CURRENT_TIME - LAST_UPDATE < FRAME_TIME){
			SDL_Delay(FRAME_TIME - CURRENT_TIME + LAST_UPDATE);
		}
	}
}
