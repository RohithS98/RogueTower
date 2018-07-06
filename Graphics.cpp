#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Graphics.h"

Graphics::Graphics(){
	SDL_CreateWindowAndRenderer(screen::SCREEN_WIDTH, screen::SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetWindowTitle(window,"RogueTower");
}

Graphics::~Graphics(){
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	std::map <std::string, SDL_Surface*> :: iterator itr;
	for( itr = spriteSheets.begin(); itr != spriteSheets.end(); ++itr){
		SDL_FreeSurface(itr->second);
	}
	std::map <std::string, TTF_Font*> :: iterator itr2;
	for( itr2 = fontList.begin(); itr2 != fontList.end(); ++itr2){
		TTF_CloseFont(itr2->second);
	}
}

SDL_Surface* Graphics::loadImage(const std::string &filePath){
	if(spriteSheets.count(filePath) == 0){
		spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return spriteSheets[filePath];
}

TTF_Font* Graphics::loadFont(const std::string &filePath, int size){
	if(fontList.count(filePath) == 0){
		fontList[filePath] = TTF_OpenFont( filePath.c_str(), size );
	}
	return fontList[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect){
	SDL_RenderCopy(renderer, texture, sourceRect, destRect);
}

void Graphics::flip(){
	SDL_RenderPresent(renderer);
}

void Graphics::clear(){
	SDL_RenderClear(renderer);
}

SDL_Renderer* Graphics::getRenderer() const{
	return renderer;
}

void Graphics::setViewPort(SDL_Rect rect){
	SDL_RenderSetViewport(renderer,&rect);
}

void Graphics::defaultViewPort(){
	SDL_Rect view = {0,0,screen::SCREEN_WIDTH, screen::SCREEN_HEIGHT};
	SDL_RenderSetViewport(renderer,&view);
}
