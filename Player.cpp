#include "Player.h"

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];

float pcSpeed = 1; // make 1 after testing
float pcJumpHeight;

Player::Player(GLfloat x, GLfloat y){
    pcX = x;
    pcY = y;
    pcWidth = 156; //pc width
    pcHeight = 256; //pc height
    pcVelocityX; //pc movement speed
    pcVelocityY;
    gravity = -0.004; //world gravity
    playerMaxX = pcX + pcWidth;
    playerMinX = pcX;
    playerMaxY = pcY + pcHeight;
    playerMinY = pcY;
    colourFlag = 1;
    health = 5;
}

void Player::playerInit(){
    pcJumpHeight = 2;

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);
}

void Player::playerUpdate(){
    moveUpdate();

    playerColliderUpdate();

    controlUpdate();
}

void Player::playerDisplay(GLuint playerTex){
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
}

void Player::playerColliderUpdate(){
    playerMaxX = pcX + pcWidth;
    playerMinX = pcX;
    playerMaxY = pcY + pcHeight;
    playerMinY = pcY;
}

void Player::moveUpdate(){
    pcX += pcVelocityX * deltaTime;
    pcY += pcVelocityY * deltaTime;
    pcVelocityY += gravity * deltaTime;
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
        if(grounded){
            pcVelocityY += pcJumpHeight;
        }
        grounded = false;
    }
}

void Player::featherGet(){
    numFeathers++;
    pcJumpHeight = 2+(0.5*numFeathers);
}

bool Player::damaged(){
    health--;
    std::cout << "health = " << health << std::endl;
    if(health <= 0){
        return true;
    }
    else{
        return false;
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