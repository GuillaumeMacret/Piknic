#include "PlayerAudio.hpp"
#include <iostream>

PlayerAudio::PlayerAudio(){
    std::string soundPath = "ressources/Sounds/pikaAtk1.wav";
    if (!pikaAtk1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaChargeSpinDash1.wav";
    if (!pikaChargeSpinDash1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaReleaseSpinDash1.wav";
    if (!pikaReleaseSpinDash1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaDieHit1.wav";
    if (!pikaDieHit1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaFall1.wav";
    if (!pikaFall1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/ringLoose1.wav";
    if (!ringLoose1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
    soundPath = "ressources/Sounds/pikaJump1.wav";
    if (!pikaJump1.loadFromFile(soundPath))std::cerr<<"Cant load "<<soundPath<<std::endl;
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

void PlayerAudio::playDieHit(){
    soundsOutput.setBuffer(pikaDieHit1);
    soundsOutput.play();
}

void PlayerAudio::playFall(){
    soundsOutput.setBuffer(pikaFall1);
    soundsOutput.play();
}

void PlayerAudio::playRingLoose(){
    soundsOutput.setBuffer(ringLoose1);
    soundsOutput.play();
}

void PlayerAudio::playJump(){
    
    soundsOutput.setBuffer(pikaJump1);
    soundsOutput.play();
}