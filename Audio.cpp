#include "Audio.hpp"
#include <iostream>

Audio::Audio(){
    std::string soundPath = "ressources/Sounds/pikaAtk1.wav";
    if (!pikaAtk1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
}

void Audio::playAtk(){
    soundsOutput.setBuffer(pikaAtk1);
    soundsOutput.play();
}