#include "Utility.h"

#ifndef GRAPHICSCWGAME_PLATFORM_H
#define GRAPHICSCWGAME_PLATFORM_H


class Platform {
    public:
        Platform(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY, bool move, int distance);

        void platformUpdate();
        void platformMove();
        void platformMoveUpdate();

        GLfloat platX;
        GLfloat platY;
        GLfloat platWidth;
        GLfloat platHeight;
        float platTexX;
        float platTexY;

        GLfloat startX;
        bool move;
        int distance;
        float platVelocityX;

        float platMinX;
        float platMaxX;
        float platMinY;
        float platMaxY;
};


#endif //GRAPHICSCWGAME_PLATFORM_H
