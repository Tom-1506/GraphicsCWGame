#include "Feather.h"

//Textures
GLuint featherTex;

Feather::Feather(GLfloat x, GLfloat y){
    fX = x;
    fY = y;
    fWidth = 128;
    fHeight = 128;
    featherMaxX = fX + fWidth;
    featherMinX = fX;
    featherMaxY = fY + fHeight;
    featherMinY = fY;
}

void Feather::featherUpdate(){
    featherMaxX = fX + fWidth;
    featherMinX = fX;
    featherMaxY = fY + fHeight;
    featherMinY = fY;
}