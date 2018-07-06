#ifndef __GAME_H__
#define __GAME_H__
#include <SDL2/SDL.h>
#include "Graphics.h"
#include "FloorMap.h"
#include "Logger.h"

namespace {
	const int FPS = 80;
	const int FRAME_TIME = 1000/FPS;
	const int FRAME_PER_MOVE = 2;
	const SDL_Rect GAME_VIEWPORT = {0,0,990,660};
	const SDL_Rect LOG_VIEWPORT = {0,660,990,160};
	const SDL_Rect STAT_VIEWPORT = {990,0,210,220};
	const SDL_Rect INVENTORY_VIEWPORT = {990,220,210,600};
}

class Game{
	public:
		Game();
		~Game();
	private:
		void gameLoop();
		void draw(Graphics &graphics);
		void update(SDL_Keycode key, int currentFrame);
		
		FloorMap fmap;
		Player player;
		Logger log;
		SDL_Keycode currentKey;
		int currentFrame;
};

#endif
