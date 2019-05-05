#include "Enemy.h"

float enemySpeed = 0.5;

Enemy::Enemy(GLfloat x, GLfloat y){
    eX = x;
    eY = y;
    enemyWidth = 240;
    enemyHeight = 380;
    eVelocityX;
    eVelocityY;
    gravity = -0.004;
    enemyMaxX = eX + enemyWidth;
    enemyMinX = eX;
    enemyMaxY = eY + enemyHeight;
    enemyMinY = eY;
    colourFlag = 1;
}

void Enemy::enemyInit(){
    facingRight = true;
}

void Enemy::enemyUpdate(){
    //std::cout << "vel: " << eVelocityY << std::endl;
    moveUpdate();
    enemyColliderUpdate();
}

void Enemy::moveUpdate(){
    //std::cout << eX << " : " << eY << " : " << enemySpeed << " : " << eVelocityY << " : " << deltaTime << std::endl;
    if(facingRight){
        eVelocityX = enemySpeed;

    }
    else{
        eVelocityX = -enemySpeed;
    }

    eX += eVelocityX * deltaTime;
    eY += eVelocityY * deltaTime;
    eVelocityY += gravity * deltaTime;

}

void Enemy::enemyColliderUpdate(){
    enemyMaxX = eX + enemyWidth;
    enemyMinX = eX;
    enemyMaxY = eY + enemyHeight;
    enemyMinY = eY;
}

void Enemy::switchDirection(){
    if(facingRight){
        facingRight = false;
    }
    else{
        facingRight = true;
    }
}