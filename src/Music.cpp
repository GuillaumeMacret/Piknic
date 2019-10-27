#include "Music.hpp"
#include <iostream>

Music::Music(){
    std::string soundPath = "ressources/Sounds/greenHillZone2.wav";
    if (!greenHill2.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
}

void Music::playMusic(int id){
    soundsOutput.setBuffer(greenHill2);
    soundsOutput.play();
}