#include <SFML/Audio.hpp>

class Music{
    public:
        sf::Sound soundsOutput;
        sf::SoundBuffer greenHill2;

        Music();
        void playMusic(int id);
};