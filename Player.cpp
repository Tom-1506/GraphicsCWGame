#include "Player.h"

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];

float pcSpeed = 1; // make 1 after testing
float pcJumpHeight;

int timeCount;
int timer = 2000;

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

    hit = false;
    timeCount = 0;
}

void Player::playerInit(){
    pcJumpHeight = 2;

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialPressed);
    glutSpecialUpFunc(keySpecialUp);

    playerTex = loadPNG((char*)"textures/enemy.png");
}

void Player::playerUpdate(){
    moveUpdate();

    playerColliderUpdate();

    controlUpdate();

    if(hit){
        timeCount += deltaTime;

        if(timeCount > timer){
            hit = false;
            timeCount = 0;
        }
    }
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
            pcVelocityY = pcJumpHeight;
        }
        grounded = false;
    }
}

void Player::featherGet(){
    numFeathers++;
    pcJumpHeight = 2+(0.2*numFeathers);
}

bool Player::damaged(){
    health--;
    hit = true;
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