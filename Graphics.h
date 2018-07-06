#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <SDL2/SDL.h>
#include <map>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct TTF_Font;

class Graphics{
	
	public:
	
	Graphics();
	~Graphics();
	SDL_Surface* loadImage(const std::string &filePath);
	TTF_Font* loadFont(const std::string &filePath, int size);
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRect, SDL_Rect* destRect);
	void flip();
	void clear();
	SDL_Renderer* getRenderer() const;
	
	private:
	
	SDL_Window window;
	SDL_Renderer renderer;
	std::map<std::string, SDL_Surface*> _spriteSheets;
	std::map<std::string, TTF_Font*> _fontList;
}

#endif
