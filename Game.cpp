#include <SDL2/SDL.h>
#include <SDL2/SDL_image>
#include <SDL2/SDL_ttf>

#include "Game.h"
#include "Actors.h"
#include "Graphics.h"
#include "Utils.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 1000/FPS;
	const int FRAME_PER_MOVE = 7;
	const int MAX_ROOMS = 10;
}

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
	Input input;
	SDL_Event e;
	
	fmap = FloorMap("ME",graphics);
	fmap.genMap(MAX_ROOMS)
