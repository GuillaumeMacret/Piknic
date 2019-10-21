#include <SFML/Audio.hpp>

class GameAudio{
    public:
        sf::Sound soundsOutput;
        sf::SoundBuffer ringPickup1;

        GameAudio();
        void playRingPickup();
};