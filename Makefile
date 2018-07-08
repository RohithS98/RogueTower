GCC = g++
CFLAGS = -g -w
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

OBJS = FloorMap.o Game.o Actors.o Utils.o Logger.o main.o Graphics.o
TARGET = RogueTower

all : $(OBJS)
	$(GCC) $(OBJS) $(LFLAGS) -o $(TARGET)

FloorMap.o : FloorMap.h Actors.h Logger.h Utils.h
Actors.o : Actors.h Graphics.h Utils.h
Utils.o : Utils.h
Logger.o : Logger.h Graphics.h
main.o : Game.h
Game.o : Game.h Graphics.h FloorMap.h Logger.h Actors.h Utils.h
Graphics.o : Graphics.h

.cpp.o:
	$(GCC) $(CFLAGS) -c $<
		
clean:
	rm -rf *.o $(TARGET)

exec:
	rm -rf *.o
