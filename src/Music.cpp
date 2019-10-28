#include "Music.hpp"
#include <iostream>

Music::Music(){
    std::string soundPath = "ressources/Sounds/greenHillZone2.ogg";
    if (!greenHill2.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/feelSunshine.ogg";
    if (!feelSunshine.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
}

void Music::playMusic(int id){
    soundsOutput.setBuffer(greenHill2);
    soundsOutput.play();
}

void Music::playSunshine(){
    soundsOutput.setBuffer(feelSunshine);
    soundsOutput.play();
}