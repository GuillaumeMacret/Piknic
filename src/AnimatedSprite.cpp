#include <SFML/Graphics.hpp>

#include "AnimatedSprite.hpp"
#include <iostream>

AnimatedSprite::AnimatedSprite():name("noname"){
	std::cerr<< "Weird default constructor call Aimated sprite"<<std::endl;
}

AnimatedSprite::AnimatedSprite(std::string name, int nbFrames, float animTime, sf::Texture textures[]):animationDuration(animTime),name(name){
	for(int i = 0; i < nbFrames;++i){
		addFrame(textures[i]);
	}
}

void AnimatedSprite::addFrame(sf::Texture frame){
	frames[frameCount++] = frame;
}

sf::Texture *AnimatedSprite::getCurrentTexture(){
	float frameDuration = animationDuration / frameCount;
	int currentFrame = (int)(timeElapsed/frameDuration);
	return &frames[currentFrame];
}

void AnimatedSprite::addTime(float time){
	timeElapsed += time;
	if(timeElapsed >= animationDuration){
		timeElapsed -= animationDuration;
	}
}

void AnimatedSprite::setAnimationDuration(float ad){
	animationDuration = ad;
}