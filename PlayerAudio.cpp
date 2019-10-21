#include "PlayerAudio.hpp"
#include <iostream>

PlayerAudio::PlayerAudio(){
    std::string soundPath = "ressources/Sounds/pikaAtk1.wav";
    if (!pikaAtk1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
}

void PlayerAudio::playAtk(){
    soundsOutput.setBuffer(pikaAtk1);
    soundsOutput.play();
}