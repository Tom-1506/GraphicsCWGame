#include "Scene.h"

GLuint playerTex; //player texture
GLuint enemyTex; //enemy texture
GLuint ground; //ground texture
GLuint feather; //feather texture
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
    loadFeathers();
    loadEnemies();
    for(Enemy e : enemies){
        e.enemyInit();
    }

    playerTex = loadPNG((char*)"textures/player.png");
    enemyTex = loadPNG((char*)"textures/enemy.png");
    ground = loadPNG((char*)"textures/girder.png");
    feather = loadPNG((char*)"textures/feather.png");
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
    for(Enemy e : enemies){
        e.enemyUpdate();
    }

    player.playerUpdate();

    sceneCollisions();
    enemySceneCollisions();
    featherCollision();
    player.playerDisplay(playerTex);

    for(Enemy e : enemies){
        e.enemyDisplay(enemyTex);
    }
    renderPlatforms();
    renderFeathers();
}

void Scene::renderPlatforms(){
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

void Scene::renderFeathers(){
    for(Feather f : feathers){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, feather);
                glColor3f(1, 0, 1);
                drawQuad(f.fX, f.fY, f.fWidth, f.fHeight, 1, 1);
            glDisable(GL_TEXTURE_2D);
            glLineWidth(15);
            glColor3f(1, player.colourFlag, player.colourFlag);
            if(drawCollisionBoxes){
                drawBox(f.fX, f.fY, f.fWidth, f.fHeight);
            }
        glPopMatrix();
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

void Scene::loadFeathers(){
    feathers.emplace_back(Feather(512, 800));
    feathers.emplace_back(Feather(2160, 800));
    feathers.emplace_back(Feather(3840, 800));
}

void Scene::featherCollision(){
    for(int i = 0; i < feathers.size(); i++){
        if(player.playerMinX < feathers[i].featherMaxX &&
           player.playerMaxX > feathers[i].featherMinX &&
           player.playerMinY < feathers[i].featherMaxY &&
           player.playerMaxY > feathers[i].featherMinY)
        {
            featherColliderLogic(i);
        }
        else{
            player.colourFlag = 1;
        }
    }
}

void Scene::featherColliderLogic(int f){
    player.colourFlag = 0;
    player.featherGet();
    feathers.erase(feathers.begin()+f);
}

void Scene::loadEnemies(){
    enemies.emplace_back(Enemy(1700, 800));
    enemies.emplace_back(Enemy(1200, 1200));
}

void Scene::enemySceneCollisions(){
    for(int i = 0; i < enemies.size(); i++){
        Enemy e = enemies[i];
        for(int j = 0; j < platformsSize; j++){
            if(e.enemyMinX < platforms[j].platMaxX &&
               e.enemyMaxX > platforms[j].platMinX &&
               e.enemyMinY < platforms[j].platMaxY &&
               e.enemyMaxY > platforms[j].platMinY)
            {
                enemySceneColliderLogic(e, j);
            }
            else{
                player.colourFlag = 1;
            }
        }
    }

}

void Scene::enemySceneColliderLogic(Enemy e, int p){
    e.colourFlag = 0;
    int smallest = 0 ;

    distTop = std::abs(e.enemyMinY - platforms[p].platMaxY);
    distBot = std::abs(e.enemyMaxY - platforms[p].platMinY);
    distRight = std::abs(e.enemyMinX - platforms[p].platMaxX);
    distLeft = std::abs(e.enemyMaxX - platforms[p].platMinX);

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
            e.eY = platforms[p].platMaxY;
            e.eVelocityY = 0;
            e.grounded = true;
            break;
        case 1:
            //std::cout << "put on bot" << std::endl;
            e.eY = platforms[p].platMinY - e.enemyHeight;
            e.eVelocityY = 0;
            e.grounded = false;
            break;
        case 2:
            //std::cout << "put on right" << std::endl;
            e.eX = platforms[p].platMaxX;
            e.eVelocityX = 0;
            break;
        case 3:
            //std::cout << "put on left" << std::endl;
            e.eX = platforms[p].platMinX - e.enemyWidth;
            e.eVelocityX = 0;
            break;
        default:
            break;
    }
}

void Scene::enemyCollision(){

}

void Scene::enemyColliderLogic(int e){

}