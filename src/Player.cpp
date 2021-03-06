#include <map>
#include <SFML/Audio.hpp>
#include <cmath>
#include <stdlib.h>
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <iostream>

#define JUMP_SPEED 1000.0f
#define VELOCITY_INC_Y 100.0f
#define VELOCITY_INC_X 10.0f
#define VELOCITY_INC_SPIN_DASH 20.0f
#define FRICTION 25.0f
#define MIN_ATK_VELOCITY 10.0f
#define KNOCKBACK_VELOCITY 10.0f
Player::Player(){}

sf::Sprite& Player::getCurrentSprite(){
    return currentSprite;
}

void Player::setCurrentSprite(sf::Sprite sprite){
    currentSprite = sprite;
}

void Player::updateAnimation(float timeElapsed){
    if(isDead){
        if(currentAnimation -> name == PIKA_DEAD){
            currentAnimation->addTime(timeElapsed);
        }else{
            currentAnimation = &animations[PIKA_DEAD];
        }
    }else if(isChargingSpinDash){
        if(facingDirection == 1){
            if(currentAnimation -> name == PIKA_CHARGE_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[PIKA_CHARGE_R];
                audioPlayer.playChargeSpinDash();
            }
        }else{
            if(currentAnimation -> name == PIKA_CHARGE_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[PIKA_CHARGE_L];
                audioPlayer.playChargeSpinDash();
            }
        }
    }else if(isAttacking){
        if(facingDirection == 1){
            if(currentAnimation -> name == PIKA_SKULLBASH_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation=&animations[PIKA_SKULLBASH_R];
                if(!isSpinDashing)audioPlayer.playAtk();
            }
        }else{
            if(currentAnimation -> name == PIKA_SKULLBASH_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation=&animations[PIKA_SKULLBASH_L];
                if(!isSpinDashing)audioPlayer.playAtk();
            }
        }
    }else if(isMoving){
        if(facingDirection == 1){
            if(currentAnimation -> name == PIKA_RUN_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[PIKA_RUN_R];
            }
        }else{
            if(currentAnimation -> name == PIKA_RUN_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[PIKA_RUN_L];
            }
        }
    }else{
        if(facingDirection == 1){
            if(currentAnimation->name == PIKA_IDLE_R){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[PIKA_IDLE_R];
            }
        }else{
            if(currentAnimation->name == PIKA_IDLE_L){
                currentAnimation->addTime(timeElapsed);
            }else{
                currentAnimation = &animations[PIKA_IDLE_L];
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
    animations[name] = AnimatedSprite(name, nbFrames, animTime,textures);
    if(currentAnimation==nullptr)currentAnimation = &animations[name];
}

void Player::resetForces(){
    moveX = moveY = 0;
}

void Player::incVelocityY(float i){
    velocityY += i;

}

void Player::incVelocity(float i){
    velocity += i;
	//std::cerr<<i<<std::endl;

    if (velocity > MAX_VELOCITY){
        velocity = MAX_VELOCITY;
    }else if(velocity < -MAX_VELOCITY){
        velocity = -MAX_VELOCITY;
    }
}

inline void Player::decVelocity(float i){
    if(velocity > 0){
        velocity -= i;
        if(velocity < 0) velocity = 0;
    }else if (velocity < 0){
        velocity += i;
    }
    
}
void Player::hasTouchedGround(){
    jumpTimer = 0;
    velocityY = 0;
}

void Player::setKeyboardForces(float timeElapsed, bool up,bool down, bool left, bool right){
    if(isDead)return;
    if((!up || !down) && isChargingSpinDash){
        isChargingSpinDash = false;
        isSpinDashing = true;
        audioPlayer.playReleaseSpinDash();
    }
    if(down){
        left = right = false;
        if(!(velocity > -MIN_ATK_VELOCITY && velocity < MIN_ATK_VELOCITY) && !isChargingSpinDash){ //roll until can't move
            isAttacking = true;
        }else if (!up && velocity != 0 && isChargingSpinDash){// Realease spin dash
            isChargingSpinDash = false;
            isSpinDashing = true;
            //FIXME group sound playing
            audioPlayer.playReleaseSpinDash();
        }else if(up && jumpTimer == 0){ // spin dash charge
            incVelocity(VELOCITY_INC_SPIN_DASH * facingDirection * timeElapsed);
            isChargingSpinDash = true;
        }/*else if (up && jumpTimer < MAXIMUM_JUMP_TIMER){
            velocityY -= VELOCITY_INC_Y *timeElapsed;
            jumpTimer += timeElapsed;
            isJumping = true;

            //moveY-=JUMP_SPEED * timeElapsed;
        }*/
    }
    if (!isChargingSpinDash && up && jumpTimer < MAXIMUM_JUMP_TIMER){
        if(!isJumping && !isSpinDashing)audioPlayer.playJump();
        velocityY -= VELOCITY_INC_Y *timeElapsed;
        jumpTimer += timeElapsed;
        isJumping = true;

        //moveY-=JUMP_SPEED * timeElapsed;
    }
    if(velocity > -MIN_ATK_VELOCITY && velocity < MIN_ATK_VELOCITY){
        isSpinDashing = false;
        isAttacking = false;
    }
    if(isSpinDashing || isChargingSpinDash){
        isAttacking = true;
    }

    if(!up || jumpTimer >= MAXIMUM_JUMP_TIMER ){
        isJumping = false;
        velocityY += VELOCITY_INC_Y * timeElapsed;
        if(velocityY > 0)velocityY = 0;
    }

    if (left){
        if(facingDirection == 1 && !down){
            isAttacking = false;
            isSpinDashing = false;
        }
        facingDirection = -1;
        if(velocity > 0){
            incVelocity((-VELOCITY_INC_X - FRICTION)* timeElapsed);
        }else{
            incVelocity(-VELOCITY_INC_X * timeElapsed);
        }
    }
    if (right) {
        if(facingDirection == -1 && !down){
            isAttacking = false;
            isSpinDashing = false;
        }
        facingDirection = 1;
        if(velocity < 0){
            incVelocity((VELOCITY_INC_X + FRICTION)* timeElapsed);
        }else{
            incVelocity(VELOCITY_INC_X* timeElapsed);
        }
    }

    // std::cerr<<velocityY<<std::endl;

    // if(velocity == 0){
    //     isMoving = false;
    // }
    // else isMoving = true;

    // std::cerr<<"vel : "<<velocity<<std::endl;

    //Fixme use forces instead
    if(!left && !right){
        isMoving = false;
    }
    else isMoving = true;
}

void Player::setVelocityInMoveX(){
    moveX = velocity;
}

void Player::setVelocityInMoveY(){
    moveY += velocityY;
}

void Player::doVelocityMove(float timeElapsed){
    if(isChargingSpinDash)return;
    lastBoundingBox = sf::FloatRect(currentSprite.getGlobalBounds());
	//std::cerr<<"Vector moving : "<<moveX<<", "<<moveY<<std::endl;
    currentSprite.move(sf::Vector2f(moveX * timeElapsed * OPTIMAL_FPS,moveY* timeElapsed * OPTIMAL_FPS));
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

void Player::setGravityForces(float timeElapsed){
    if(!isJumping)moveY += GRAVITY /OPTIMAL_FPS;
}

void Player::setFrictionForces(float timeElapsed){
    if(isMoving || isChargingSpinDash)return;
    decVelocity(FRICTION * timeElapsed);
}

void Player::getHit(){
    //Don't damage in iframe
    if(IframeCounter <= I_FRAME_DUR || isDead)return;
    if(ringCounter <= 0){
        std::cout<<"[INFO] Player got gameover by hit"<<std::endl;
        audioPlayer.playDieHit();
        isDead = true;
        //exit(0);
    }else{
        //TODO spread rings
        ringCounter = 0;
        IframeCounter = 0;
        velocity = KNOCKBACK_VELOCITY * -facingDirection;
        audioPlayer.playRingLoose();
    }
}

void Player::fallFromMap(){
    if(!isDead){
        audioPlayer.playFall();
        isDead = true;
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
