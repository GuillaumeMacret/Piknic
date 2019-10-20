#include <SFML/Audio.hpp>

class Audio{
    public:
        sf::Sound soundsOutput;
        sf::SoundBuffer pikaAtk1;

        Audio();
        void playAtk();
};