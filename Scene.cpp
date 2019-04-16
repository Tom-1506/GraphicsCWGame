#include "Scene.h"
#include "Platform.h"

GLuint ground; //ground texture
GLuint background; //background texture

int worldWidth = 7680;
int worldHeight = 4320;
int blockDim = 128;

Player player = Player(512, 512);

Platform platforms[] = {Platform(0, 0, worldWidth, blockDim, worldWidth/blockDim, 1), //floor
                        Platform(0, blockDim, blockDim, worldHeight, 1, worldHeight/blockDim), //left wall
                        Platform(worldWidth-blockDim, blockDim, blockDim, worldHeight, 1, worldHeight/blockDim), //right wall
                        Platform(768, 400, blockDim, blockDim, 1, 1),
                        Platform(1028, 768, 4*blockDim, blockDim, 4, 1),
                        Platform(2000, blockDim, 8*blockDim, blockDim, 8, 1),
                        Platform(2500, 600, 2*blockDim, blockDim, 2, 1),
                        Platform(3100, 1000, blockDim, blockDim, 1, 1),
                        Platform(2200, 1400, 5*blockDim, blockDim, 5, 1)};
int platformsSize = (sizeof(platforms)/sizeof(*platforms));

float distTop;
float distBot;
float distRight;
float distLeft;

float dists[4];
int distsSize = (sizeof(dists)/sizeof(*dists));

Scene::Scene(){
}

void Scene::sceneInit(){
    player.playerInit();
    ground = loadPNG((char*)"textures/girder.png");
    background = loadPNG((char*)"textures/grid_background.png");
}

void Scene::sceneUpdate(){
    glTranslatef(-player.pcX+1780, -player.pcY+952, 0);

    //background
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, background);
            glColor3f(0, 1, 0);
            drawQuad(-1920, -1080, 1.5*worldWidth, 2*worldHeight, 45, 34);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    for(Platform p : platforms){
        p.platformUpdate();
    }

    player.playerUpdate();
    sceneCollisions();
    player.playerDisplay();

    renderPlatforms();
}

void renderPlatforms(){
    for(Platform p : platforms){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    for(int i=1;  i < distsSize; i++){
        if(dists[i] < dists[smallest]){
            smallest = i;
        }
    }

    //std::cout << "smallest: " << smallest << std::endl;

    switch(smallest){
        case 0:
            //std::cout << "put on top" << std::endl;
            player.pcY = platforms[p].platMaxY;
            player.pcVelocityY = 0;
            player.grounded = true;
            break;
        case 1:
            //std::cout << "put on bot" << std::endl;
            player.pcY = platforms[p].platMinY - player.pcHeight;
            player.pcVelocityY = 0;
            player.grounded = false;
            break;
        case 2:
            //std::cout << "put on right" << std::endl;
            player.pcX = platforms[p].platMaxX;
            player.pcVelocityX = 0;
            break;
        case 3:
            //std::cout << "put on left" << std::endl;
            player.pcX = platforms[p].platMinX - player.pcWidth;
            player.pcVelocityX = 0;
            break;
        default:
            break;
    }
}

void Scene::sceneCollisions(){
    for(int i = 0; i < platformsSize; i++){
        if(player.playerMinX < platforms[i].platMaxX &&
           player.playerMaxX > platforms[i].platMinX &&
           player.playerMinY < platforms[i].platMaxY &&
           player.playerMaxY > platforms[i].platMinY)
        {
            sceneColliderLogic(i);
        }
        else{
            player.colourFlag = 1;
        }
    }
}

/*
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
}*/
