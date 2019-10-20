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

void Enemy::doMove(){
    currentSprite.move(sf::Vector2f(moveX,moveY));
}

void Enemy::setMovementForces(float timeElapsed){
    moveX += ENEMY_BASE_SPEED * direction * timeElapsed;
}

bool Enemy::isColliding(sf::FloatRect box){
    return currentSprite.getGlobalBounds().intersects(box);
}

void Enemy::setGravityForces(float timeElapsed){
    moveY += GRAVITY * timeElapsed;
}

void Enemy::resetForces(){
    moveX = moveY = 0;
}

bool Enemy::handleWallCollision(sf::FloatRect box){
    resetForces();

    sf::FloatRect playerBox = currentSprite.getGlobalBounds();
    //Collision is on top of the box
    float topDelta = (playerBox.top + playerBox.height) - box.top;
    //Collision is on left of the box
    float leftDelta = (playerBox.left + playerBox.width) - box.left;
    //Collision is on right of the box
    float rightDelta = playerBox.left - (box.left + box.width);
    //Collision is on bottom of the box
    float botDelta = playerBox.top - (box.top + box.height);

    if(topDelta > 0 && topDelta < box.height / COLLISION_DETECTION_DIVIDER){
        moveY = -topDelta;
    }

    if(leftDelta > 0 && leftDelta < box.width / COLLISION_DETECTION_DIVIDER){
        moveX = -leftDelta;
        direction = -direction;
    }

    if(rightDelta < 0 && rightDelta > -(box.width / COLLISION_DETECTION_DIVIDER) && topDelta > box.width / COLLISION_DETECTION_DIVIDER){
        moveX = -rightDelta;
        direction = -direction;
    }

    if(botDelta < 0 && botDelta > -(box.height / COLLISION_DETECTION_DIVIDER)){
        //FIXME abnormal *2
        moveY = -botDelta*2;
    }

    if(moveX || moveY){
        doMove();
    }
}