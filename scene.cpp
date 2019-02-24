#include "scene.h"

GLuint ground; //ground texture
GLuint background; //background texture

/*bool drawCollisionBoxes = true;*/

void sceneInit(){
    ground = loadPNG((char*)"textures/magic_dirt.png");
    background = loadPNG((char*)"textures/ray_background.png");
}

void sceneUpdate(){
    //background
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, background);
            glColor3f(0, 1, 0);
            drawQuad(1920, 1080, 3840, 2160, 1, 1);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //ground
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, ground);
            glColor3f(0, 0, 0);
            drawQuad(1920, 128, 3840, 256, 15, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, 1, 1);
        //if(drawCollisionBoxes){
            drawBox(1920, 128, 3840, 256);
        //}
    glPopMatrix();
}