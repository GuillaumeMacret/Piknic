#include "PlayerAudio.hpp"
#include "AnimatedSprite.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

class Player{
    public:

        int ringCounter = 0;
        float moveX = 0,moveY = 0,x,y;
        float jumpTimer = 0, IframeCounter;

        PlayerAudio audioPlayer;
        sf::FloatRect lastBoundingBox;
        bool isMoving = false, isJumping = false,isAttacking = false, isChargingSpinDash = false, isSpinDashing = false, isDead = false, isFalling = false;
        int facingDirection = 1;
        float velocity = 0, velocityY = 0, xMovementCollision = 0;
        sf::Sprite currentSprite;
        AnimatedSprite *currentAnimation;
        std::map<std::string, AnimatedSprite> animations;

        Player();
        sf::Sprite &getCurrentSprite();
        void setCurrentSprite(sf::Sprite sprite);
        void resetForces();
        void setKeyboardForces(float timeElapsed, bool up,bool down, bool left, bool right);
        void setVelocityInMoveX();
        void setVelocityInMoveY();
        void hasTouchedGround();
        void setFrictionForces(float timeElapsed);
        void setGravityForces(float timeElapsed);
        void doMove();
        void doVelocityMove();
        sf::FloatRect *getMovementRect();
        sf::FloatRect *getCurrentRect();
        sf::FloatRect *getLastRect();
        bool isColliding(sf::FloatRect box);
        int isMovementColliding(sf::FloatRect box);
        bool handleWallCollision(sf::FloatRect box);
        bool handleMovementCollision(sf::FloatRect box);
        bool handleEnemyCollision(sf::FloatRect box);
        bool handleRingCollision(sf::FloatRect box);
        void getHit();
        void fallFromMap();
        void updateIframe(float timeElapsed);
        void incVelocity(float i);
        void incVelocityY(float i);
        void decVelocity(float i);
        int createNewAnimation(std::string name, int nbFrames, float animTime, sf::Texture textures[]);
        void updateAnimation(float timeElapsed);
};