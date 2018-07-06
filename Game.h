#ifndef __GAME_H__
#define __GAME_H__

#include "graphics.h"
#include "FloorMap.h"
#include "Logger.h"

class Game{
	public:
		Game();
		~Game();
		gameLoop();
		draw(Graphics &graphics);
		update();
	private:
		void gameLoop();
		void draw(Graphics &graphics);
		void update(float elapsedTime);
		
		FloorMap fmap;
		Player player;
		SDL_Keycode currentKey;
		int currentFrame;
}

#endif
