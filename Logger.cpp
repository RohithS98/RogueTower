#include "Logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Takes 2 names
 * Format : A attacks B
*/
void Logger::logAttack(std::string a,std::string b){
	std::stringstream s;
	s.str("");
	s<<a<<" attacks "<<b;
	logAdd(s.str());
}

/* Takes 2 names and damage
 * Format : A inflicted D damage on B
*/
void Logger::logDamage(std::string a,std::string b,int d){
	std::stringstream s;
	s.str("");
	s<<a<<" inflicted "<<d<<" damage on "<<b;
	logAdd(s.str());
}

/* Takes 1 name
 * Format : A faded away
*/
void Logger::logKilled(std::string a){
	std::stringstream s;
	s.str("");
	s<<a<<" faded away";
	logAdd(s.str());
}

/* Takes floor
 * Format : Entered Level L
*/
void Logger::logNewFloor(int l){
	std::stringstream s;
	s.str("");
	s<<"Entered Level "<<l;
	logAdd(s.str());
}

//Clears the log
void Logger::clearLog(){
	logMessages.clear();
}

//Adds a ... to log. Use for new line
void Logger::logLine(){
	logAdd("...");
}

/* Takes name
 * Format : A missed
*/
void Logger::logMiss(std::string a){
	std::stringstream s;
	s.str("");
	s<<a<<" missed";
	logAdd(s.str());
}

void Logger::logXP(int xp){
	std::stringstream s;
	s.str("");
	s<<"Gained "<<xp<<" XP";
	logAdd(s.str());
}

void Logger::logLevelUp(int level){
	std::stringstream s;
	s.str("");
	s<<"Grew to level "<<level;
	logAdd(s.str());
}

//Renders the log messages using graphics. Newest messages are at bottom
void Logger::render(Graphics &graphics){
	TTF_Font* gFont = graphics.loadFont(Log::fontName,Log::size);
	SDL_Rect bounds = {0,0,990,160};
	graphics.drawRect(bounds,0x0,0x0,0x0);
	for(int i = 0; i < logMessages.size(); i++){
		graphics.blitText(gFont,logMessages[i],Log::logColor,5, 160 + 15*(i-logMessages.size()));
	}
}

//Adds message to the message queue. Deletes extra messages
void Logger::logAdd(std::string logm){
	logMessages.push_back(logm);
	if(logMessages.size() > Log::MAXLOGMESSAGES){
		logMessages.pop_front();
	}
}
