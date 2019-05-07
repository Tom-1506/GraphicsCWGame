#include "Utility.h"

#ifndef GRAPHICSCWGAME_BUTTON_H
#define GRAPHICSCWGAME_BUTTON_H


class Button {
    public:
        Button(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY);

        void buttonUpdate();

        GLfloat bX;
        GLfloat bY;
        GLfloat buttonWidth;
        GLfloat buttonHeight;
        float buttonTexX;
        float buttonTexY;
        int colourFlag;

        float buttonMinX;
        float buttonMaxX;
        float buttonMinY;
        float buttonMaxY;
};


#endif //GRAPHICSCWGAME_BUTTON_H
