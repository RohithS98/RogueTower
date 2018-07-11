#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]){
	#ifdef DEBUG
	std::cout<<"Starting game"<<std::endl;
	#endif
	Game game;
	#ifdef DEBUG
	std::cout<<"Exiting game"<<std::endl;
	#endif
	return 0;
}
