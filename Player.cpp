#include <SFML/Audio.hpp>
#include <cmath>
#include <stdlib.h>
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <iostream>

#define SPEED 500
#define GRAVITY 500
#define JUMP_SPEED 1000
#define MAXIMUM_JUMP_TIMER .15f
#define VELOCITY_INC 10
#define FRICTION 50
#define MIN_ATK_VELOCITY 10

Player::Player(){
    
}

sf::Sprite& Player::getCurrentSprite(){
    return currentSprite;
}

void Player::setCurrentSprite(sf::Sprite sprite){
    currentSprite = sprite;
}

void Player::updateAnimation(float timeElapsed){
    //FIXME This is static => this is bad
    if(isAttacking){
        if(facingDirection == 1){
            if(currentAnimation -> name == PIKA_SKULLBASH_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation=&animations[4];
                audioPlayer.playAtk();
            }
        }else{
            if(currentAnimation -> name == PIKA_SKULLBASH_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation=&animations[5];
                audioPlayer.playAtk();
            }
        }
    }else if(isMoving){
        if(facingDirection == 1){
            if(currentAnimation -> name == PIKA_RUN_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[2];
            }
        }else{
            if(currentAnimation -> name == PIKA_RUN_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[3];
            }
        }
    }else{
        if(facingDirection == 1){
            if(currentAnimation->name == PIKA_IDLE_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[0];
            }
        }else{
            if(currentAnimation->name == PIKA_IDLE_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[1];
            }
        }
        
    }
    currentSprite.setTexture(*(currentAnimation->getCurrentTexture()),true);
    //std::cerr<<"currentAnim is : "<<currentAnimation->name<<std::endl;
}

/**
 * Returns the id of the animation
 **/
int Player::createNewAnimation(std::string name, int nbFrames, float animTime, sf::Texture textures[]){
    AnimatedSprite newAnim(name, nbFrames, animTime,textures);
    animations.push_back(newAnim);
    if(currentAnimation==nullptr)currentAnimation = &animations[0];
}

void Player::resetForces(){
    moveX = moveY = 0;
}

inline void Player::incVelocity(float i){
    velocity += i;
    if (velocity > MAX_VELOCITY){
        velocity = MAX_VELOCITY;
    }
}

inline void Player::decVelocity(float i){
    velocity -= i;
    if(velocity < 0) velocity = 0;
}

void Player::setKeyboardForces(float timeElapsed, bool up,bool down, bool left, bool right){
    if (left){
        facingDirection = -1;
        incVelocity(VELOCITY_INC * timeElapsed);
    }
    if (right) {
        facingDirection = 1;
        incVelocity(VELOCITY_INC * timeElapsed);
    }
    if (up && jumpTimer < MAXIMUM_JUMP_TIMER){
        moveY-=JUMP_SPEED * timeElapsed;
        jumpTimer += timeElapsed;
        isJumping = true;
    }
    if(!up || jumpTimer >= MAXIMUM_JUMP_TIMER ){
        isJumping = false;
    }
    if(down && velocity > MIN_ATK_VELOCITY){
        isAttacking = true;
    }else{
        isAttacking = false;
    }
    
    //Fixme use forces instead
    if(!left && !right){
        isMoving = false;
    }
    else isMoving = true;
}

void Player::setVelocityInMoveX(){
    moveX = velocity * facingDirection;
}

void Player::doVelocityMove(){
    lastBoundingBox = sf::FloatRect(currentSprite.getGlobalBounds());
	std::cerr<<"Vector moving : "<<moveX<<", "<<moveY<<std::endl;
    currentSprite.move(sf::Vector2f(moveX,moveY));
}

sf::FloatRect *Player::getMovementRect(){
    sf::FloatRect *leftBox, *rightBox, *topBox, *botBox;
    sf::FloatRect currentBoundingBox(currentSprite.getGlobalBounds());
    if(currentBoundingBox.left < lastBoundingBox.left){
         leftBox = &currentBoundingBox;
         rightBox = &lastBoundingBox;
    }else{
        leftBox = &lastBoundingBox;
        rightBox = &currentBoundingBox;
    }

    float left = leftBox->left;
    float width = (rightBox->left+rightBox->width) - leftBox->left;

    if(currentBoundingBox.top < lastBoundingBox.top){
        topBox = &currentBoundingBox;
        botBox = &lastBoundingBox;
    }else{
        topBox = &lastBoundingBox;
        botBox = &currentBoundingBox;
    }

    float top = topBox -> top;
    float height = (botBox->top + botBox -> height) - topBox->top;

    //sf::FloatRect *rect = new sf::FloatRect(leftBox->left,leftBox->top,(rightBox->left+rightBox->width) - leftBox->left, leftBox->top -rightBox->top+rightBox->height);
    sf::FloatRect *rect = new sf::FloatRect(left,top,width,height);

    return rect;
}


sf::FloatRect *Player::getCurrentRect(){
    sf::FloatRect *rect = new sf::FloatRect(currentSprite.getGlobalBounds());
    return rect;
}

sf::FloatRect *Player::getLastRect(){
    sf::FloatRect *rect = new sf::FloatRect(lastBoundingBox);
    return rect;
}

void Player::doMove(){
    //lastBoundingBox = currentSprite.getGlobalBounds();
    currentSprite.move(sf::Vector2f(moveX,moveY));
}

void Player::setGravityForces(float timeElapsed){
    if(!isJumping)moveY += GRAVITY * timeElapsed;
}

void Player::setFrictionForces(float timeElapsed){
    if(!isMoving)decVelocity(FRICTION * timeElapsed);
}



// bool Player::isColliding(sf::FloatRect box){
//     return currentSprite.getGlobalBounds().intersects(box);
// }

// int Player::isMovementColliding(sf::FloatRect box){
//     sf::FloatRect *movementBoundingBox = getMovementRect();
//     if(!movementBoundingBox->intersects(box))return NO_COLLISION;

//     //Collision is on top of the box
//     float topDelta = (lastBoundingBox.top + lastBoundingBox.height) - box.top;
//     //Collision is on left of the box
//     float leftDelta = (lastBoundingBox.left + lastBoundingBox.width) - box.left;
//     //Collision is on right of the box
//     float rightDelta = lastBoundingBox.left - (box.left + box.width);
//     //Collision is on bottom of the box
//     float botDelta = lastBoundingBox.top - (box.top + box.height);

//     std::cout << topDelta << " "<< botDelta << " "<< leftDelta << " "<< rightDelta << std::endl;

//     if(abs(topDelta) < abs(leftDelta) && abs(topDelta) < abs(rightDelta)){
//         return TOP_COLLISION;
//     }

//     return RIGHT_COLLISION;
// }



// bool Player::handleMovementCollision(sf::FloatRect box){
//     //Get this movement
//     int dirMul = 1;
//     sf::FloatRect *leftBox, *rightBox;
//     if(box.left < lastBoundingBox.left){
//          leftBox = &box;
//          rightBox = &lastBoundingBox;
//          dirMul = -1;
//          std::cout<<"Player is right, box is left"<<std::endl;
//     }else{
//         leftBox = &lastBoundingBox;
//         rightBox = &box;
//          std::cout<<"Player is left, box is right"<<std::endl;
//     }
    
//     float offset = rightBox->left - (leftBox -> left + leftBox -> width);
//     std::cout<<"Offset :"<<offset<<std::endl;
//     //If the tile is closser (movementX less than previous, store it)
//     if(abs(offset) < abs(moveX)){
//         moveX =  offset * dirMul;
//         std::cout<<"new moveX "<<moveX<<std::endl;
//     }
    
// }

// /**
//  * TODO do custom collide box
//  **/
// bool Player::handleWallCollision(sf::FloatRect box){
//     sf::FloatRect playerBox = currentSprite.getGlobalBounds();
//     //Collision is on top of the box
//     float topDelta = (playerBox.top + playerBox.height) - box.top;
//     //Collision is on left of the box
//     float leftDelta = (playerBox.left + playerBox.width) - box.left;
//     //Collision is on right of the box
//     float rightDelta = playerBox.left - (box.left + box.width);
//     //Collision is on bottom of the box
//     float botDelta = playerBox.top - (box.top + box.height);

//     if(topDelta > 0 && topDelta < box.height / COLLISION_DETECTION_DIVIDER){
//         moveY = -topDelta;
//         jumpTimer = 0;
//     }

//     if(leftDelta > 0 && leftDelta < box.width / COLLISION_DETECTION_DIVIDER){
//         moveX = -leftDelta;
//     }

//     if(rightDelta < 0 && rightDelta > -(box.width / COLLISION_DETECTION_DIVIDER) && topDelta > box.width / COLLISION_DETECTION_DIVIDER){
//         moveX = -rightDelta;
//     }

//     if(botDelta < 0 && botDelta > -(box.height / COLLISION_DETECTION_DIVIDER)){
//         //FIXME abnormal *2
//         moveY = -botDelta*2;
//     }
// }

void Player::getHit(){
    //Don't damage in iframe
    if(IframeCounter <= I_FRAME_DUR)return;
    if(ringCounter <= 0){
        std::cerr<<"Player got gameover"<<std::endl;
        //exit(0);
    }else{
        //TODO spread rings
        ringCounter = 0;
        IframeCounter = 0;
    }
}

void Player::updateIframe(float timeElapsed){
    if(IframeCounter < I_FRAME_DUR)IframeCounter+=timeElapsed;
}

bool Player::handleEnemyCollision(sf::FloatRect box){
    if(isAttacking){
        return true;
    }else{
        return false;
    }
}

bool Player::handleRingCollision(sf::FloatRect box){
    if(IframeCounter<I_FRAME_DUR){
        return false;
    }else{
        ringCounter++;
        return true;
    }
}
