#include <SFML/Audio.hpp>

class PlayerAudio{
    public:
        sf::Sound soundsOutput;
        sf::SoundBuffer pikaAtk1, pikaChargeSpinDash1,pikaReleaseSpinDash1, pikaDieHit1, pikaFall1;

        PlayerAudio();
        void playAtk();
        void playChargeSpinDash();
        void playReleaseSpinDash();
        void playDieHit();
        void playFall();
};