#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "SOIL.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>

#ifndef GRAPHICSCWGAME_UTILITY_H
#define GRAPHICSCWGAME_UTILITY_H

class Utility{
};

void updateTime();
void drawBox(float posX, float posY, float w, float h);
void drawQuad(float posX, float posY, float w, float h, int texRepeatX, int texRepeatY);
GLuint loadPNG(char* name);

extern bool drawCollisionBoxes;
extern double deltaTime;

#endif //GRAPHICSCWGAME_UTILITY_H
