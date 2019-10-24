#ifndef ANIMATED_SPRITE_INCLUDE
#define ANIMATED_SPRITE_INCLUDE

#define MAX_FRAME_ANIM 32
#include <SFML/Graphics.hpp>

class AnimatedSprite{
	public:
		std::string name;
		float timeElapsed = 0;
		int frameCount = 0;
		//In seconds
		float animationDuration;
		sf::Texture frames[MAX_FRAME_ANIM];
		AnimatedSprite();
		AnimatedSprite(std::string name, int nbFrames, float animTime, sf::Texture textures[]);
		void addFrame(sf::Texture frame);
		sf::Texture *getCurrentTexture();

		void addTime(float time);
		void setAnimationDuration(float ad);
};

#endif
