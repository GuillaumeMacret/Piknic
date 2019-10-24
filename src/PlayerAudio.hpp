#include <SFML/Audio.hpp>

class PlayerAudio{
    public:
        sf::Sound soundsOutput;
        sf::SoundBuffer pikaAtk1, pikaChargeSpinDash1,pikaReleaseSpinDash1;

        PlayerAudio();
        void playAtk();
        void playChargeSpinDash();
        void playReleaseSpinDash();
};