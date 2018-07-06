#include "Logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void Logger::logAttack(std::string a,std::string b){
	std::stringstream s;
	s.str("");
	s<<a<<" attacks "<<b;
	logAdd(s.str());
}

void Logger::logDamage(std::string a,std::string b,int d){
	std::stringstream s;
	s.str("");
	s<<a<<" inflicted "<<d<<" damage on "<<b;
	logAdd(s.str());
}

void Logger::logKilled(std::string a){
	std::stringstream s;
	s.str("");
	s<<a<<" faded away";
	logAdd(s.str());
}

void Logger::logNewFloor(int l){
	std::stringstream s;
	s.str("");
	s<<"Entered Level "<<l;
	logAdd(s.str());
}

void Logger::clearLog(){
	logMessages.clear();
}

void Logger::logLine(){
	logAdd("...");
}

void Logger::logMiss(std::string a){
	std::stringstream s;
	s.str("");
	s<<a<<" missed";
	logAdd(s.str());
}

void Logger::render(Graphics &graphics){
	TTF_Font* gFont = graphics.loadFont(Log::fontName,Log::size);
	SDL_Rect bounds = {0,0,990,160};
	graphics.drawRect(bounds,0x0,0x0,0x0);
	for(int i = 0; i < logMessages.size(); i++){
		graphics.blitText(gFont,logMessages[i],Log::logColor,5, 160 + 15*(i-logMessages.size()));
	}
}

void Logger::logAdd(std::string logm){
	logMessages.push_back(logm);
	if(logMessages.size() > Log::MAXLOGMESSAGES){
		logMessages.pop_front();
	}
}
