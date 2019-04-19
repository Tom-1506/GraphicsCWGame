#include "Utility.h"
#ifndef GRAPHICSCWGAME_FEATHER_H
#define GRAPHICSCWGAME_FEATHER_H


class Feather {
public:
    Feather(GLfloat x, GLfloat y);

    void featherUpdate();

    GLfloat fX; //feather x position
    GLfloat fY; //feather y position
    GLfloat fWidth;
    GLfloat fHeight;
    float featherMaxX;
    float featherMinX;
    float featherMaxY;
    float featherMinY;
};


#endif //GRAPHICSCWGAME_FEATHER_H
