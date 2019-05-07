#include "Enemy.h"

float enemySpeed = 0.5;

Enemy::Enemy(GLfloat x, GLfloat y){
    eX = x;
    eY = y;
    enemyWidth = 240;
    enemyHeight = 380;
    eVelocityX = 0;
    eVelocityY = 0;
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
    moveUpdate();
    enemyColliderUpdate();
}

void Enemy::moveUpdate(){
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