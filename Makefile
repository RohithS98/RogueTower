GCC = g++
CFLAGS = -g -w
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

OBJS = FloorMap.o Game.o Actors.o Utils.o Logger.o main.o Graphics.o
TARGET = RogueTower

all : $(OBJS)
	$(GCC) $(OBJS) $(LFLAGS) -o $(TARGET)

testmap.o: FloorMap.h
FloorMap.o : FloorMap.h
LTexture.o: LTexture.h
LTimer.o: LTimer.h
Actors.o : Actors.h
LText.o : LText.h
Utils.o : Utils.h
Logger.o : Logger.h
main.o : Game.h
Game.o : Game.h

.cpp.o:
	$(GCC) $(CFLAGS) -c $<
		
clean:
	rm -rf *.o $(TARGET)

exec:
	rm -rf *.o
