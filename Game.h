#ifndef __GAME_H__
#define __GAME_H__

#include "Graphics.h"
#include "FloorMap.h"
#include "Logger.h"

namespace {
	const int FPS = 10;
	const int FRAME_TIME = 1000/FPS;
	const int FRAME_PER_MOVE = 7;
	const int MAX_ROOMS = 10;
}

class Game{
	public:
		Game();
		~Game();
	private:
		void gameLoop();
		void draw(Graphics &graphics);
		void update(float elapsedTime);
		
		FloorMap fmap;
		Player player;
		SDL_Keycode currentKey;
		int currentFrame;
};

#endif
