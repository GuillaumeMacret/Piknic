#include "PlayerAudio.hpp"
#include <iostream>

PlayerAudio::PlayerAudio(){
    std::string soundPath = "ressources/Sounds/pikaAtk1.wav";
    if (!pikaAtk1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaChargeSpinDash1.wav";
    if (!pikaChargeSpinDash1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaReleaseSpinDash1.wav";
    if (!pikaReleaseSpinDash1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
}

void PlayerAudio::playAtk(){
    soundsOutput.setBuffer(pikaAtk1);
    soundsOutput.play();
}

void PlayerAudio::playChargeSpinDash(){
    soundsOutput.setBuffer(pikaChargeSpinDash1);
    soundsOutput.play();
}

void PlayerAudio::playReleaseSpinDash(){
    soundsOutput.setBuffer(pikaReleaseSpinDash1);
    soundsOutput.play();
}