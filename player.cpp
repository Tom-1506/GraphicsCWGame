#include "player.h"

//Variable definitions
GLfloat pcX = 512; //player character X position
GLfloat pcY = 512; //pc Y position
GLfloat pcWidth = 160; //pc width
GLfloat pcHeight = 256; //pc height
GLfloat pcTransX = 0; //pc X translate
GLfloat pcTransY = 0; //pc Y translate
float pcSpeed = 10.0f; //pc movement speed
float gravity = 14; //world gravity

//Textures
GLuint player;

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];

void playerInit(){
    player = loadPNG((char*)"textures/player.png");

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);
}

void playerUpdate(){
    //Player character
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, player);
            glTranslatef(pcTransX, pcTransY, 0);
            glColor3f(1, 0, 0);
            drawQuad(pcX, pcY, pcWidth, pcHeight, 1, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, 1, 1);
        if(drawCollisionBoxes){
            drawBox(pcX, pcX, pcWidth, pcHeight);
        }
    glPopMatrix();

    //pcTransY -= gravity;

    controlUpdate();
}

void controlUpdate(){
    //Movement Keys
    if(keySpecialStates[GLUT_KEY_LEFT]){
        pcTransX -= pcSpeed;
    }
    if(keySpecialStates[GLUT_KEY_RIGHT]){
        pcTransX += pcSpeed;
    }
}

void keyPressed(unsigned char key, int x, int y){
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y){
    keyStates[key] = false;
}

void keySpecialPressed (int key, int x, int y) {
    keySpecialStates[key] = true;
}

void keySpecialUp (int key, int x, int y) {
    keySpecialStates[key] = false;
}