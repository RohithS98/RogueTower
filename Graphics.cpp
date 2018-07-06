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
}

SDL_Surface* Graphics::loadImage(const std::string &filePath){
	if(spriteSheets.count(filePath) == 0){
		spriteSheets[filePath] = IMG_Load(filePath.c_str());
		SDL_SetColorKey( spriteSheets[filePath], SDL_TRUE, SDL_MapRGB( spriteSheets[filePath]->format,0,0xFF,0xFF) );
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

void Graphics::drawRect(SDL_Rect rect, int r, int g, int b){
	SDL_SetRenderDrawColor(renderer,r,g,b,0xFF);
	SDL_RenderFillRect(renderer,&rect);
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

void Graphics::blitText(TTF_Font* gFont,const std::string text, SDL_Color col, int x, int y){
	SDL_Surface* textSurf = TTF_RenderText_Solid( gFont, text.c_str(), col);
	SDL_Rect source = {0, 0, textSurf->w, textSurf->h};
	SDL_Rect dest = {x, y, textSurf->w, textSurf->h};
	blitSurface(SDL_CreateTextureFromSurface(renderer ,	textSurf),&source,&dest);
	SDL_FreeSurface( textSurf );
}

void Graphics::blitTextCenterX(TTF_Font* gFont,const std::string text, SDL_Color col, int xmax, int y, int xoff){
	SDL_Surface* textSurf = TTF_RenderText_Solid( gFont, text.c_str(), col);
	SDL_Rect source = {0, 0, textSurf->w, textSurf->h};
	SDL_Rect dest = {(xmax-textSurf->w)/2 + xoff, y, textSurf->w, textSurf->h};
	blitSurface(SDL_CreateTextureFromSurface(renderer ,	textSurf),&source,&dest);
	SDL_FreeSurface( textSurf );
}
