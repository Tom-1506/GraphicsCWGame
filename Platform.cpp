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

void Platform::platformMove(){
    if(movingRight && platX > startX+distance){
        platVelocityX = -platSpeed;
        movingRight = false;
    }
    if(!movingRight && platX < startX){
        platVelocityX = platSpeed;
        movingRight = true;
    }
    platformMoveUpdate();
}

void Platform::platformMoveUpdate(){
    //std::cout << "move: " << platVelocityX  << " " << deltaTime<< std::endl;
    platX += (platVelocityX * deltaTime);
}