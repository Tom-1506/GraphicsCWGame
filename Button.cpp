#include "Button.h"

Button::Button(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY){
    bX = posX;
    bY = posY;
    buttonWidth = w;
    buttonHeight = h;
    buttonTexX = texRepeatX;
    buttonTexY = texRepeatY;

    buttonMinX = bX;
    buttonMaxX = bX + buttonWidth;
    buttonMinY = bY;
    buttonMaxY = bY + buttonHeight;
}

void Button::buttonUpdate(){
}