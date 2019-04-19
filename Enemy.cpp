#include "Enemy.h"

float enemySpeed = 1;

Enemy::Enemy(GLfloat x, GLfloat y){
    eX = x;
    eY = y;
    enemyWidth = 184;
    enemyHeight = 228;
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
    std::cout <<"init"<<std::endl;
}

void Enemy::enemyUpdate(){
    moveUpdate();

    enemyColliderUpdate();
}

void Enemy::enemyDisplay(GLuint enemyTex){
    //Enemy
    std::cout << "display   " << eY << std::endl;
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, enemyTex);
            glTranslatef(eX, eY, 0);
            glColor3f(1, 0, 0);
            drawQuad(0, 0, enemyWidth, enemyHeight, 1, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, colourFlag, colourFlag);
        if(drawCollisionBoxes){
            drawBox(0, 0, enemyWidth, enemyHeight);
        }
    glPopMatrix();
}

void Enemy::moveUpdate(){
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

