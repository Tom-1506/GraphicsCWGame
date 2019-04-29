#include "Platform.h"

Platform::Platform(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY, bool move, int distance, float speed){
    platX = posX;
    platY = posY;
    startX = posX;
    platWidth = w;
    platHeight = h;
    platTexX = texRepeatX;
    platTexY = texRepeatY;
    this->distance = distance;
    this->move = move;
    movingRight = true;
    platSpeed = speed;

    platMinX = platX;
    platMaxX = platX + platWidth;
    platMinY = platY;
    platMaxY = platY + platHeight;
}

void Platform::platformUpdate(){
    platMinX = platX;
    platMaxX = platX + platWidth;
    platMinY = platY;
    platMaxY = platY + platHeight;
    if(move){
        if(movingRight){
            platVelocityX = platSpeed;
        }
        else{
            platVelocityX = -platSpeed;
        }
        platformMove();
    }
}

void Platform::platformDisplay(){
    //Enemy
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTranslatef(platX, platY, 0);
            glColor3f(0, 0, 1);
            drawQuad(0, 0, platWidth, platHeight, 1, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(0, 0, 1);
        if(drawCollisionBoxes){
            drawBox(0, 0, platWidth, platHeight);
        }
    glPopMatrix();
}

void Platform::platformMove(){
    if(movingRight && platX > startX+distance){
        std::cout << "switch left" << std::endl;
        platVelocityX = -platSpeed;
        movingRight = false;
    }
    if(!movingRight && platX < startX){
        std::cout << "switch right" << std::endl;
        platVelocityX = platSpeed;
        movingRight = true;
    }
    platformMoveUpdate();
}

void Platform::platformMoveUpdate(){
    platX += platVelocityX * deltaTime;
}