#include "Platform.h"

float platSpeed = 1;

Platform::Platform(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY, bool move, int distance){
    platX = posX;
    platY = posY;
    startX = posX;
    platWidth = w;
    platHeight = h;
    platTexX = texRepeatX;
    platTexY = texRepeatY;
    this->distance = distance;
    this->move = move;

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
        platVelocityX = platSpeed;
        platformMove();
    }
}

void Platform::platformMove(){
    std::cout << platX << std::endl;
    if(platX > startX+distance){
        platVelocityX = -platSpeed;
        std::cout << "vel: " << platVelocityX << std::endl;
    }
    if(platX < startX){
        platVelocityX = platSpeed;
        std::cout << "vel: " << platVelocityX << std::endl;
    }
    platformMoveUpdate();
}

void Platform::platformMoveUpdate(){
    std::cout << platX << " before\n";
    platX += platVelocityX * deltaTime;
    std::cout << platX << std::endl;
}