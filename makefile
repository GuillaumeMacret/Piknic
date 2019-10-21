CXXFLAGS = -std=c++11

OBJS = GameAudio.o PlayerAudio.o Enemy.o MyClock.o AnimatedSprite.o Player.o main.o 

LIBS = -lpthread -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

TARGET = piknic.app
CXX = g++

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)
	$(CXX) $(COPTION) -c $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf *.*~
	rm -rf *~

Enemy.o : Enemy.hpp Enemy.cpp
	c++ -c Enemy.cpp -o Enemy.o

MyClock.o : MyClock.hpp MyClock.cpp
	c++ -c MyClock.cpp -o MyClock.o

Player.o: Player.hpp Player.cpp
	c++ -c Player.cpp -o Player.o

PlayerAudio.o: PlayerAudio.hpp PlayerAudio.cpp
	c++ -c PlayerAudio.cpp -o PlayerAudio.o

GameAudio.o: GameAudio.hpp GameAudio.cpp
	c++ -c GameAudio.cpp -o GameAudio.o

AnimatedSprite.o: AnimatedSprite/AnimatedSprite.hpp AnimatedSprite/AnimatedSprite.cpp
	c++ -c AnimatedSprite/AnimatedSprite.cpp -o AnimatedSprite.o

PikachuWalkingRightAnim.o : AnimatedSprite/Pikachu/PikachuWalkingRightAnim.cpp AnimatedSprite/Pikachu/PikachuWalkingRightAnim.hpp
	c++ -c AnimatedSprite/Pikachu/PikachuWalkingRightAnim.cpp -o PikachuWalkingRightAnim.o
