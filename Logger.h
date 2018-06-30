#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <deque>
#include <sstream>
using namespace std;

const int MAXLOGMESSAGES = 10;

class Logger{
	public:
	void logAttack(string,string);
	void logDamage(string,string,int);
	void logKilled(string);
	void logNewFloor(int);
	void clearLog();
	void logLine();
	void logMiss(string);
	deque<string> logMessages;
	private:
	void logAdd(string);
};

#endif
