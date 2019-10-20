#include <SFML/Graphics.hpp>

#include "AnimatedSprite.hpp"
#include <iostream>

AnimatedSprite::AnimatedSprite(std::string name, int nbFrames, float animTime, sf::Texture textures[]):animatioDuration(animTime),name(name){
	for(int i = 0; i < nbFrames;++i){
		addFrame(textures[i]);
	}
}

void AnimatedSprite::addFrame(sf::Texture frame){
	frames[frameCount++] = frame;
}

sf::Texture *AnimatedSprite::getCurrentTexture(){
	float frameDuration = animatioDuration / frameCount;
	int currentFrame = (int)(timeElapsed/frameDuration);
	return &frames[currentFrame];
}

void AnimatedSprite::addTime(float time){
	timeElapsed += time;
	if(timeElapsed >= animatioDuration){
		timeElapsed -= animatioDuration;
	}
}

void AnimatedSprite::setAnimationDuration(float ad){
	animatioDuration = ad;
}