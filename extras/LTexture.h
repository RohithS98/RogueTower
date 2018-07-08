#ifndef __LTEXTURE_H__
#define __LTEXTURE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( SDL_Renderer* gRenderer, std::string path );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( SDL_Renderer* gRenderer, int x, int y,int mW = 0, int mH = 0, SDL_Rect* clip = NULL);
		
		void setClips(int clipNo, SDL_Rect clipArray[]);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		SDL_Rect* clipList;
	
		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif
