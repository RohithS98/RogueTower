#include "Item.h"

bool Item::isSpriteLoaded = false;
SDL_Texture* Item::spriteSheet = NULL;

Item::Item(){
    #ifdef DEBUG
    std::cout<<"Creating Item";
    #endif
}

Item::Item(int x, int y, int type, int level){
    this->x = x;
    this->y = y;
    this->type = type;
    this->strength = getStrength(level);
    #ifdef DEBUG
    std::cout<<"Creating Item spec"<<std::endl;
    #endif
}

Item::~Item(){
    #ifdef DEBUG
    std::cout<<"Deleting Item"<<std::endl;
    #endif
}

void Item::draw(Graphics &graphics){
    setSprite(graphics, "resources/itemsprite.png", (this->type%ItemConst::SPRITES_PER_LINE)*100, (this->type/ItemConst::SPRITES_PER_LINE)*100, 100, 100);
    float halfScale = (ItemConst::ITEM_SCALE - 1.0)/2;
	SDL_Rect destRect = {(y-halfScale)*global::TILE_SIZE,(x-halfScale)*global::TILE_SIZE,global::TILE_SIZE*ItemConst::ITEM_SCALE,global::TILE_SIZE*ItemConst::ITEM_SCALE};
    graphics.blitSurface( Item::spriteSheet, &sourceRect, &destRect );
}

void Item::setSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height){
    sourceRect.x = sourceX;
	sourceRect.y = sourceY;
	sourceRect.w = width;
	sourceRect.h = height;
    if(!isSpriteLoaded){
        Item::spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
        Item::isSpriteLoaded = true;
    }
}

void Item::freeSprites(){
    SDL_DestroyTexture(spriteSheet);
    spriteSheet = NULL;
    isSpriteLoaded = false;
}

int Item::getStrength(int level){
    switch (this->type) {
        case atkUp:case defUp:case luckUp:
        return 2 + level/5;
        case potions:case potionl:
        return 3*level;
        case healthUp:
        return 3 + level/5;
    }
}
