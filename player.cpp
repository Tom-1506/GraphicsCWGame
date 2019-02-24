#include "player.h"

//Variable definitions
GLfloat pcX = 512; //player character X position
GLfloat pcY = 512; //player character Y position
GLfloat pcTransX = 0; //player character X transform
GLfloat pcTransY = 0; //player character Y transform

float pcSpeed = 10.0f; // speed of pc

//Textures
GLuint player;

void playerInit(){
    player = loadPNG((char*)"textures/player.png");
}

void playerUpdate(){
    //Player character
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, player);
            glTranslatef(pcTransX, pcTransY, 0);
            glColor3f(1, 0, 0);
            drawQuad(pcX, pcY, 256, 256, 1, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, 1, 1);
        //if(drawCollisionBoxes){
        drawBox(512, 512, 256, 256);
        //}
    glPopMatrix();
}