#include <iostream>
#include "Enemy.hpp"
#include "Constants.hpp"

Enemy::Enemy(/* args */){
}

Enemy::Enemy(float x, float y){
    currentSprite.move(sf::Vector2f(x,y));
}

Enemy::~Enemy(){
}

void Enemy::doMove(float timeElapsed){
    currentSprite.move(sf::Vector2f(moveX,moveY));
}

void Enemy::setMovementForces(float timeElapsed){
    moveX = ENEMY_BASE_SPEED * direction * timeElapsed;
}

bool Enemy::isColliding(sf::FloatRect box){
    return currentSprite.getGlobalBounds().intersects(box);
}

void Enemy::setGravityForces(float timeElapsed){
    moveY += GRAVITY*timeElapsed;
}

void Enemy::resetForces(){
    moveX = moveY = 0;
}