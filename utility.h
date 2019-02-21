#include <GL/freeglut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "SOIL.h"

#include <iostream>
using namespace std;

#ifndef GRAPHICSCWGAME_UTILITY_H
#define GRAPHICSCWGAME_UTILITY_H


class utility {

};

void drawBox(float centreX, float centreY, float w, float h);
void drawQuad(float centreX, float centreY, float w, float h, int texRepeatX, int texRepeatY);
GLuint loadPNG(char* name);

#endif //GRAPHICSCWGAME_UTILITY_H
