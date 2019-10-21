#include <SFML/Audio.hpp>

class PlayerAudio{
    public:
        sf::Sound soundsOutput;
        sf::SoundBuffer pikaAtk1;

        PlayerAudio();
        void playAtk();
};