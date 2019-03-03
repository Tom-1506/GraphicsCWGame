#include "Scene.h"
#include "Player.h"
#include "Platform.h"

GLuint ground; //ground texture
GLuint background; //background texture

Player player = Player(512, 512);
Platform platforms[2] = {Platform(0, 0, 3840, 256, 15, 1),
                         Platform(768, 554, 256, 256, 1, 1)};

float distTop;
float distBot;
float distRight;
float distLeft;

float dists[4];

Scene::Scene(){
}

void Scene::sceneInit(){
    player.playerInit();
    ground = loadPNG((char*)"textures/magic_dirt.png");
    background = loadPNG((char*)"textures/ray_background.png");
    std::cout << "size: " << sizeof(dists) << std::endl;
}

void Scene::sceneUpdate(){
    //background
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, background);
            glColor3f(0, 1, 0);
            drawQuad(0, 0, 3840, 2160, 1, 1);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    renderPlatforms();

    player.playerUpdate();
    for(Platform p : platforms){
        p.platformUpdate();
    }

    sceneCollisions();
}

void renderPlatforms(){
    for(Platform p : platforms){
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, ground);
                glColor3f(0, 0, 0);
                drawQuad(p.platX, p.platY, p.platWidth, p.platHeight, p.platTexX, p.platTexY);
            glDisable(GL_TEXTURE_2D);
            glLineWidth(15);
            glColor3f(1, player.colourFlag, player.colourFlag);
            if(drawCollisionBoxes){
                drawBox(p.platX, p.platY, p.platWidth, p.platHeight);
            }
        glPopMatrix();
    }
}

void Scene::sceneColliderLogic(int p){
    player.colourFlag = 0;
    int smallest = 0 ;

    distTop = std::abs(player.playerMinY - platforms[p].platMaxY);
    distBot = std::abs(player.playerMaxY - platforms[p].platMinY);
    distRight = std::abs(player.playerMinX - platforms[p].platMaxX);
    distLeft = std::abs(player.playerMaxX - platforms[p].platMinX);

    //std::cout << "Top: " << distTop << " - Bottom: " << distBot << " - Right: " << distRight << " - Left: " << distLeft << std::endl;

    dists[0] = distTop;
    dists[1] = distBot;
    dists[2] = distRight;
    dists[3] = distLeft;



    for(int i=1;  i < sizeof(dists); i++){
        if (dists[i] < dists[smallest]){
            smallest = i;
        }
    }

    std::cout << "smallest: " << smallest << std::endl;

    switch(smallest){
        case 0:
            std::cout << "put on top" << std::endl;
            player.pcY = platforms[p].platMaxY;
            player.pcVelocityY = 0;
            break;
        case 1:
            std::cout << "put on bot" << std::endl;
            player.pcY = platforms[p].platMinY - player.pcHeight;
            player.pcVelocityY = 0;
            break;
        case 2:
            std::cout << "put on right" << std::endl;
            player.pcX = platforms[p].platMaxX;
            player.pcVelocityX = 0;
            break;
        case 3:
            std::cout << "put on left" << std::endl;
            player.pcX = platforms[p].platMinX - player.pcWidth;
            player.pcVelocityX = 0;
            break;
        default:
            break;
    }
}

void Scene::sceneCollisions(){
    if(player.playerMinX < platforms[0].platMaxX &&
       player.playerMaxX > platforms[0].platMinX &&
       player.playerMinY < platforms[0].platMaxY &&
       player.playerMaxY > platforms[0].platMinY)
    {
        sceneColliderLogic(0);
    }
    else if(player.playerMinX < platforms[1].platMaxX &&
            player.playerMaxX > platforms[1].platMinX &&
            player.playerMinY < platforms[1].platMaxY &&
            player.playerMaxY > platforms[1].platMinY)
    {
        sceneColliderLogic(1);
    }
    else{
        player.colourFlag = 1;
    }
}