#include "Platform.h"

Platform::Platform(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY){
    platX = posX;
    platY = posY;
    platWidth = w;
    platHeight = h;
    platTexX = texRepeatX;
    platTexY = texRepeatY;

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
}