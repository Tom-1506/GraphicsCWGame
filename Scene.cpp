#include "Scene.h"
#include "Player.h"

GLuint ground; //ground texture
GLuint background; //background texture

GLfloat groundX = 1920;
GLfloat groundY = 128;
GLfloat groundWidth = 3840;
GLfloat groundHeight = 256;

Player player = Player(512, 512);

Scene::Scene(){
}

void Scene::sceneInit(){
    player.playerInit();
    ground = loadPNG((char*)"textures/magic_dirt.png");
    background = loadPNG((char*)"textures/ray_background.png");
}

void Scene::sceneUpdate(){
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
            drawQuad(groundX, groundY, groundWidth, groundHeight, 15, 1);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, player.colourFlag, player.colourFlag);
        if(drawCollisionBoxes){
            drawBox(1920, 128, 3840, 256);
        }
    glPopMatrix();

    player.playerUpdate();

    sceneColliderUpdate();

    sceneCollisions();
}

void Scene::sceneColliderUpdate(){
    groundMaxX = groundX + groundWidth/2;
    groundMinX = groundX - groundWidth/2;
    groundMaxY = groundY + groundHeight/2;
    groundMinY = groundY - groundHeight/2;
}

void Scene::sceneCollisions(){
    if(player.playerMinX < groundMaxX &&
       player.playerMaxX > groundMinX &&
       player.playerMinY < groundMaxY &&
       player.playerMaxY > groundMinY)
    {
        player.colourFlag = 0;
        player.pcVelocityY = 0;
        player.grounded = true;
    }
    else{
        player.colourFlag = 1;
        player.grounded = false;
    }
}