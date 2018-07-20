#ifndef __ITEM_H__
#define __ITEM_H__

#include <iostream>
#include "Graphics.h"
#include "Utils.h"

enum ItemType{
    atkUp,
    defUp,
    luckUp,
    potions,
    potionl,
    healthUp,
};

namespace ItemConst{
    const float ITEM_SCALE = 1.4;
    const int SPRITES_PER_LINE = 5;
}

class Item{
public:
    int x,y,type,strength;
    Item();
    Item(int x, int y, int type, int level);
    ~Item();
    void draw(Graphics &graphics);
    void setSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height);
    static void freeSprites();
private:
    static bool isSpriteLoaded;
    SDL_Rect sourceRect;
	static SDL_Texture* spriteSheet;
    int getStrength(int level);
};

#endif
