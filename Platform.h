#include "Utility.h"

#ifndef GRAPHICSCWGAME_PLATFORM_H
#define GRAPHICSCWGAME_PLATFORM_H


class Platform {
    public:
        Platform(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY);

        void platformUpdate();

        GLuint platX;
        GLuint platY;
        GLuint platWidth;
        GLuint platHeight;
        GLuint platTexX;
        GLuint platTexY;

        float platMinX;
        float platMaxX;
        float platMinY;
        float platMaxY;
};


#endif //GRAPHICSCWGAME_PLATFORM_H
