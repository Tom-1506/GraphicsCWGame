#include "Player.h"

//Textures
GLuint playerTex;

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];

float pcSpeed = 1;
float pcJumpHeight = 2;

Player::Player(GLfloat x, GLfloat y){
    pcX = x;
    pcY = y;
    pcWidth = 160; //pc width
    pcHeight = 256; //pc height
    pcVelocityX; //pc movement speed
    pcVelocityY;
    gravity = -0.004; //world gravity
    playerMaxX = pcX + pcWidth/2;
    playerMinX = pcX - pcWidth/2;
    playerMaxY = pcY + pcHeight/2;
    playerMinY = pcY - pcHeight/2;
    colourFlag = 1;
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
            glTranslatef(pcX, pcY, 0);
            glColor3f(1, 0, 0);
            drawQuad(0, 0, pcWidth, pcHeight, 1, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, colourFlag, colourFlag);
        if(drawCollisionBoxes){
            drawBox(0, 0, pcWidth, pcHeight);
        }
    glPopMatrix();

    playerColliderUpdate();

    moveUpdate();

    controlUpdate();
}

void Player::playerColliderUpdate(){
    playerMaxX = pcX + pcWidth/2;
    playerMinX = pcX - pcWidth/2;
    playerMaxY = pcY + pcHeight/2;
    playerMinY = pcY - pcHeight/2;
}

void Player::moveUpdate(){
    pcX += pcVelocityX * deltaTime;
    pcY += pcVelocityY * deltaTime;
    if(!grounded){
        pcVelocityY += gravity * deltaTime;
    }
}

void Player::controlUpdate(){
    //Movement Keys
    if(!keySpecialStates[GLUT_KEY_LEFT]){
        pcVelocityX = 0;
    }
    if(!keySpecialStates[GLUT_KEY_RIGHT]){
        pcVelocityX = 0;
    }
    if(keySpecialStates[GLUT_KEY_LEFT]){
        pcVelocityX = -pcSpeed;
    }
    if(keySpecialStates[GLUT_KEY_RIGHT]){
        pcVelocityX = pcSpeed;
    }
    if(keySpecialStates[GLUT_KEY_UP]){
        std::cout << "jump" << std::endl;
        if(grounded){
            pcVelocityY += pcJumpHeight;
        }
    }
}

void keyPressed(unsigned char key, int x, int y){
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y){
    keyStates[key] = false;
}

void keySpecialPressed(int key, int x, int y){
    keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y){
    keySpecialStates[key] = false;
}