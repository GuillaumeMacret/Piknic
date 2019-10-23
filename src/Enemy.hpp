#include <SFML/Graphics.hpp>

#define ENEMY_BASE_SPEED 400

class Enemy{
private:
    /* data */
public:
    bool spriteInited = false;
    float moveX=0,moveY=0;
    int direction = 1;
    sf::Sprite currentSprite;


    Enemy(/* args */);
    Enemy(float x, float y);
    ~Enemy();

    void doMove();
    bool isColliding(sf::FloatRect box);
    void setGravityForces(float timeElapsed);
    void setMovementForces(float timeElapsed);
    bool handleWallCollision(sf::FloatRect box);
    void resetForces();
};
