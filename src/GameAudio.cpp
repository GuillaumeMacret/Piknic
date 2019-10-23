#include "GameAudio.hpp"
#include <iostream>

GameAudio::GameAudio(){
    std::string ringPickup1Path = "ressources/Sounds/ringPickup1.wav";
    if (!ringPickup1.loadFromFile(ringPickup1Path))std::cerr<<"Cant load "<<ringPickup1Path<<std::endl;
}

void GameAudio::playRingPickup(){
    soundsOutput.setBuffer(ringPickup1);
    soundsOutput.play();
}