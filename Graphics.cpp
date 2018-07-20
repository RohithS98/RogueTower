#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Graphics.h"

//Creates the window and renderer. Sets game name.
Graphics::Graphics(){
	#ifdef DEBUG
	std::cout<<"Initializing graphics"<<std::endl;
	#endif
	SDL_CreateWindowAndRenderer(screen::SCREEN_WIDTH, screen::SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetWindowTitle(window,"RogueTower");
}

//Destroys the renderer and window
Graphics::~Graphics(){
	#ifdef DEBUG
	std::cout<<"Stoping graphics"<<std::endl;
	#endif
	for (std::map<std::string, TTF_Font*>::iterator it = fontList.begin(); it!= fontList.end(); it++){
		//std::cout<<it->first<<" "<<it->second<<std::endl;
		TTF_CloseFont(it->second);
	}
	for (std::map<std::string, SDL_Surface*>::iterator it = spriteSheets.begin(); it!= spriteSheets.end(); it++){
		//sstd::cout<<it->first<<" "<<it->second<<std::endl;
		SDL_FreeSurface(it->second);
	}
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
}

//Returns surface for the image. Load image to memory only once
SDL_Surface* Graphics::loadImage(const std::string &filePath){
	if(spriteSheets.count(filePath) == 0){
		#ifdef DEBUG
		std::cout<<"Loading Image :"<<filePath<<std::endl;
		#endif
		spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return spriteSheets[filePath];
}

//Returns font for a ttf file. Loads font only once
TTF_Font* Graphics::loadFont(const std::string &filePath, int size){
	if(fontList.count(filePath) == 0){
		#ifdef DEBUG
		std::cout<<"Loading Font :"<<filePath<<std::endl;
		#endif
		fontList[filePath] = TTF_OpenFont( filePath.c_str(), size );
	}
	return fontList[filePath];
}

//Blits a part of texture onto a part of the screen
void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect){
	SDL_RenderCopy(renderer, texture, sourceRect, destRect);
}

//Draws a rectangle on the screen
void Graphics::drawRect(SDL_Rect rect, int r, int g, int b){
	SDL_SetRenderDrawColor(renderer,r,g,b,0xFF);
	SDL_RenderFillRect(renderer,&rect);
}

//Renders all changes on screen
void Graphics::flip(){
	SDL_RenderPresent(renderer);
}

//Clear the renderer/window
void Graphics::clear(){
	SDL_RenderClear(renderer);
}

//Returns the renderer
SDL_Renderer* Graphics::getRenderer() const{
	return renderer;
}

//Sets the Viewport
void Graphics::setViewPort(SDL_Rect rect){
	SDL_RenderSetViewport(renderer,&rect);
}

//Sets default Viewport
void Graphics::defaultViewPort(){
	SDL_Rect view = {0,0,screen::SCREEN_WIDTH, screen::SCREEN_HEIGHT};
	SDL_RenderSetViewport(renderer,&view);
}

//Blits text using a font and given string and colour onto the screen
void Graphics::blitText(TTF_Font* gFont,const std::string text, SDL_Color col, int x, int y){
	SDL_Surface* textSurf = TTF_RenderText_Solid( gFont, text.c_str(), col);
	SDL_Rect source = {0, 0, textSurf->w, textSurf->h};
	SDL_Rect dest = {x, y, textSurf->w, textSurf->h};
	SDL_Texture* texttex = SDL_CreateTextureFromSurface(renderer ,	textSurf);
	blitSurface(texttex,&source,&dest);
	SDL_DestroyTexture(texttex);
	SDL_FreeSurface( textSurf );
}
//Blits text using a font and given string and colour onto the screen(centered along X)
void Graphics::blitTextCenterX(TTF_Font* gFont,const std::string text, SDL_Color col, int xmax, int y, int xoff){
	SDL_Surface* textSurf = TTF_RenderText_Solid( gFont, text.c_str(), col);
	SDL_Rect source = {0, 0, textSurf->w, textSurf->h};
	SDL_Rect dest = {(xmax-textSurf->w)/2 + xoff, y, textSurf->w, textSurf->h};
	SDL_Texture* texttex = SDL_CreateTextureFromSurface(renderer ,	textSurf);
	blitSurface(texttex,&source,&dest);
	SDL_DestroyTexture(texttex);
	SDL_FreeSurface( textSurf );
}
