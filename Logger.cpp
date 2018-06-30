#include "Logger.h"

void Logger::logAttack(string a,string b){
	stringstream s;
	s.str("");
	s<<a<<" attacks "<<b;
	logAdd(s.str());
}

void Logger::logDamage(string a,string b,int d){
	stringstream s;
	s.str("");
	s<<a<<" inflicted "<<d<<" damage on "<<b;
	logAdd(s.str());
}

void Logger::logKilled(string a){
	stringstream s;
	s.str("");
	s<<a<<" faded away";
	logAdd(s.str());
}

void Logger::logNewFloor(int l){
	stringstream s;
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

void Logger::logMiss(string a){
	stringstream s;
	s.str("");
	s<<a<<" missed";
	logAdd(s.str());
}

void Logger::logAdd(string logm){
	logMessages.push_back(logm);
	if(logMessages.size() > MAXLOGMESSAGES){
		logMessages.pop_front();
	}
}
