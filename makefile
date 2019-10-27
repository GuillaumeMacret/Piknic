CXXFLAGS = -std=c++11

OBJS = bin/GameAudio.o bin/PlayerAudio.o bin/Enemy.o bin/MyClock.o bin/AnimatedSprite.o bin/Player.o bin/main.o 

LIBS = -lpthread -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

TARGET = piknic.test
BUILD = piknic
CXX = g++

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

build:	makeDir $(OBJS)
	
	$(CXX) -o $(BUILD) $(OBJS) $(LIBS)

all:	$(TARGET)
	$(CXX) $(COPTION) -c $(TARGET)

makeDir:
	mkdir bin || echo "Bin exists ignore above error"

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf *.*~
	rm -rf *~
	rm -rf *.o

bin/main.o : src/main.cpp
	c++ -c src/main.cpp -o bin/main.o

bin/Enemy.o : src/Enemy.hpp src/Enemy.cpp
	c++ -c src/Enemy.cpp -o bin/Enemy.o

bin/MyClock.o : src/MyClock.hpp src/MyClock.cpp
	c++ -c src/MyClock.cpp -o bin/MyClock.o

bin/Player.o: src/Player.hpp src/Player.cpp
	c++ -c src/Player.cpp -o bin/Player.o

bin/PlayerAudio.o: src/PlayerAudio.hpp src/PlayerAudio.cpp
	c++ -c src/PlayerAudio.cpp -o bin/PlayerAudio.o

bin/GameAudio.o: src/GameAudio.hpp src/GameAudio.cpp
	c++ -c src/GameAudio.cpp -o bin/GameAudio.o

bin/AnimatedSprite.o: src/AnimatedSprite.hpp src/AnimatedSprite.cpp
	c++ -c src/AnimatedSprite.cpp -o bin/AnimatedSprite.o