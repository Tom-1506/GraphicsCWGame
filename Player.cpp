#include "Player.h"

//Textures
GLuint playerTex;

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];

Player::Player(GLfloat x, GLfloat y){
    pcX = x;
    pcY = y;
    pcWidth = 160; //pc width
    pcHeight = 256; //pc height
    pcTransX = 0; //pc X translate
    pcTransY = 0; //pc Y translate
    pcSpeed = 10.0f; //pc movement speed
    gravity = 14; //world gravity
}

void Player::playerInit(){
    playerTex = loadPNG((char*)"textures/player.png");

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);
}

void Player::playerUpdate(){
    //Player character
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, playerTex);
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

void Player::controlUpdate(){
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

void keySpecialPressed (int key, int x, int y){
    keySpecialStates[key] = true;
}

void keySpecialUp (int key, int x, int y){
    keySpecialStates[key] = false;
}