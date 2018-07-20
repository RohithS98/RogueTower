#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <deque>
#include <sstream>
#include "Graphics.h"

struct SDL_Color;

namespace Log{
	const int MAXLOGMESSAGES = 10;
	const std::string fontName = "resources/Raleway-Medium.ttf";
	const int size = 12;
	const SDL_Color logColor = {0xFF,0xFF,0xFF,0xFF};
}

class Logger{
	public:
	void logAttack(std::string,std::string);
	void logDamage(std::string,std::string,int);
	void logKilled(std::string);
	void logNewFloor(int);
	void clearLog();
	void logLine();
	void logMiss(std::string);
	void logXP(int xp);
	void logLevelUp(int level);
	void logCrit(std::string,std::string,int);
	void logStatA();
	void logStatD();
	void logStatH();
	void logStatL();
	void logHeal(int);
	std::deque<std::string> logMessages;
	void render(Graphics &graphics);
	private:
	void logAdd(std::string);
};

#endif
