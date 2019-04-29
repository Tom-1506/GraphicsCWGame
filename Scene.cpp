#include "Scene.h"

GLuint playerTex; //player texture
GLuint enemyTex; //enemy texture
GLuint ground; //ground texture
GLuint feather; //feather texture
GLuint background; //background texture
GLuint health;
GLuint died;

float enemyHitSpeedY = 1;
float enemyHitSpeedX = 1;

int worldWidth = 7680;
int worldHeight = 4320;
int blockDim = 128;

bool playerAlive = true;
Player player = Player(1200, 1200);

Platform platforms[] = {Platform(3600, 0, 3*blockDim, blockDim, 3, 1, false, 0, 0),
                        Platform(1028, 768, 4*blockDim, blockDim, 4, 1, false, 0, 0),
                        Platform(2000, blockDim, 8*blockDim, blockDim, 8, 1, false, 0, 0),
                        Platform(2500, 600, 2*blockDim, blockDim, 2, 1, false, 0, 0),
                        Platform(3100, 1000, blockDim, blockDim, 1, 1, false, 0, 0),
                        Platform(2200, 1400, 5*blockDim, blockDim, 5, 1, false, 0, 0),
                        Platform(4500, 256, 7*blockDim, blockDim, 7, 1, false, 0, 0),
                        Platform(5840, 700, 4*blockDim, blockDim, 4, 1, false, 0, 0)};
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
    loadMovingPlatforms();
    player.playerInit();
    loadFeathers();
    loadEnemies();
    for(int i = 0; i < enemies.size(); i++){
        enemies[i]->enemyInit();
    }

    playerTex = loadPNG((char*)"textures/player.png");
    enemyTex = loadPNG((char*)"textures/enemy.png");
    ground = loadPNG((char*)"textures/girder.png");
    feather = loadPNG((char*)"textures/feather.png");
    background = loadPNG((char*)"textures/grid_background.png");
    health = loadPNG((char*) "textures/magic_dirt.png");
    died = loadPNG((char*) "textures/you-died.png");
}

void Scene::sceneUpdate(){
    glTranslatef(-player.pcX+1740, -player.pcY+952, 0);

    //background
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, background);
            glColor3f(0, 1, 0);
            drawQuad(-1920, -1080, 1.5*worldWidth, 2*worldHeight, 45, 34);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //health bar
    glLoadIdentity();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, health);
            glColor3f(0, 1, 0);
            drawQuad(100, 1900, 100*player.health, 100, 1, 1);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glTranslatef(-player.pcX+1740, -player.pcY+952, 0);


    for(Platform p : platforms){
        p.platformUpdate();
    }
    for(int i = 0; i < movingPlatforms.size(); i++){
        movingPlatforms[i]->platformUpdate();
    }
    for(int i = 0; i < enemies.size(); i++){
        enemies[i]->enemyUpdate();
    }

    if(playerAlive){
        player.playerUpdate();
        if(player.pcY < -1000){
            playerAlive = false;
        }
    }

    sceneCollisions();
    movingSceneCollisions();
    enemySceneCollisions();
    featherCollision();

    if(playerAlive){
        enemyCollision();
        player.playerDisplay(playerTex);
    }

    for(int i = 0; i < enemies.size(); i++){
        enemies[i]->enemyDisplay(enemyTex);
    }
    for(int i = 0; i < movingPlatforms.size(); i++){
        movingPlatforms[i]->platformDisplay();
    }
    renderPlatforms();
    renderFeathers();

    //dead
    if(!playerAlive){
        glLoadIdentity();
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, died);
                glColor3f(0, 1, 0);
                drawQuad(0, 850, 3840, 400, 1, 1);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void Scene::loadMovingPlatforms(){
    Platform p = Platform(400, 400, blockDim, blockDim, 1, 1, true, 400, 0.3);
    Platform* pp = new Platform(p.platX, p.platY, p.platWidth, p.platHeight, p.platTexX, p.platTexY, p.move, p.distance, p.platSpeed);
    movingPlatforms.emplace_back(pp);
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

    dists[0] = distTop;
    dists[1] = distBot;
    dists[2] = distRight;
    dists[3] = distLeft;

    for(int i=1;  i < distsSize; i++){
        if(dists[i] < dists[smallest]){
            smallest = i;
        }
    }

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

void Scene::movingSceneCollisions(){
    for(int i = 0; i < movingPlatforms.size(); i++){
        Platform* p = movingPlatforms[i];
        if(player.playerMinX < p->platMaxX &&
           player.playerMaxX > p->platMinX &&
           player.playerMinY < p->platMaxY &&
           player.playerMaxY > p->platMinY)
        {
            movingSceneColliderLogic(i);
        }
        else{
            player.colourFlag = 1;
        }
    }
}

void Scene::movingSceneColliderLogic(int p){
    Platform* plat = movingPlatforms[p];
    player.colourFlag = 0;
    int smallest = 0 ;

    distTop = std::abs(player.playerMinY - plat->platMaxY);
    distBot = std::abs(player.playerMaxY - plat->platMinY);
    distRight = std::abs(player.playerMinX - plat->platMaxX);
    distLeft = std::abs(player.playerMaxX - plat->platMinX);

    dists[0] = distTop;
    dists[1] = distBot;
    dists[2] = distRight;
    dists[3] = distLeft;

    for(int i=1;  i < distsSize; i++){
        if(dists[i] < dists[smallest]){
            smallest = i;
        }
    }

    switch(smallest){
        case 0:
            //std::cout << "put on top" << std::endl;
            player.pcY = plat->platMaxY;
            player.pcVelocityY = 0;
            player.grounded = true;
            break;
        case 1:
            //std::cout << "put on bot" << std::endl;
            player.pcY = plat->platMinY - player.pcHeight;
            player.pcVelocityY = 0;
            player.grounded = false;
            break;
        case 2:
            //std::cout << "put on right" << std::endl;
            player.pcX = plat->platMaxX;
            player.pcVelocityX = 0;
            break;
        case 3:
            //std::cout << "put on left" << std::endl;
            player.pcX = plat->platMinX - player.pcWidth;
            player.pcVelocityX = 0;
            break;
        default:
            break;
    }
}

void Scene::loadFeathers(){
    feathers.emplace_back(Feather(6200, 1000));
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
    Enemy e = Enemy(0, 0);
    Enemy* ep = new Enemy(0, 0);
    enemies.emplace_back(ep);

    e = Enemy(1800, 512);
    ep = new Enemy(e.eX, e.eY);
    enemies.emplace_back(ep);

    e = Enemy(4800, 600);
    ep = new Enemy(e.eX, e.eY);
    enemies.emplace_back(ep);

    e = Enemy(2450, 1450);
    ep = new Enemy(e.eX, e.eY);
    enemies.emplace_back(ep);
}

void Scene::enemySceneCollisions(){
    for(int i = 0; i < enemies.size(); i++){
        Enemy* e = enemies[i];
        for(int j = 0; j < platformsSize; j++){
            if(e->enemyMinX < platforms[j].platMaxX &&
               e->enemyMaxX > platforms[j].platMinX &&
               e->enemyMinY < platforms[j].platMaxY &&
               e->enemyMaxY > platforms[j].platMinY)
            {
                enemySceneColliderLogic(e, j);
            }
            else{
                player.colourFlag = 1;
            }
        }
    }

}

void Scene::enemySceneColliderLogic(Enemy* e, int p){
    e->colourFlag = 0;
    int smallest = 0 ;

    distTop = std::abs(e->enemyMinY - platforms[p].platMaxY);
    distBot = std::abs(e->enemyMaxY - platforms[p].platMinY);
    distRight = std::abs(e->enemyMinX - platforms[p].platMaxX);
    distLeft = std::abs(e->enemyMaxX - platforms[p].platMinX);

    dists[0] = distTop;
    dists[1] = distBot;
    dists[2] = distRight;
    dists[3] = distLeft;

    for(int i=1;  i < distsSize; i++){
        if(dists[i] < dists[smallest]){
            smallest = i;
        }
    }

    //std::cout << "Enemy: " << e->eX << "Top: " << distTop << " - Bottom: " << distBot << " - Right: " << distRight << " - Left: " << distLeft << std::endl;
    //std::cout << "smallest: " << smallest << std::endl;

    switch(smallest){
        case 0:
            //std::cout << "put on top" << std::endl;
            e->eY = platforms[p].platMaxY;
            e->eVelocityY = 0;
            e->grounded = true;
            if(e->grounded){
                if((e->eX + e->enemyWidth/2) > platforms[p].platMaxX){
                    e->switchDirection();
                }
                if((e->eX + e->enemyWidth/2) < platforms[p].platMinX){
                    e->switchDirection();
                }
            }
            break;
        case 1:
            //std::cout << "put on bot" << std::endl;
            e->eY = platforms[p].platMinY - e->enemyHeight;
            e->eVelocityY = 0;
            e->grounded = false;
            break;
        case 2:
            //std::cout << "put on right" << std::endl;
            e->eX = platforms[p].platMaxX;
            e->eVelocityX = 0;
            e->switchDirection();
            break;
        case 3:
            //std::cout << "put on left" << std::endl;
            e->eX = platforms[p].platMinX - e->enemyWidth;
            e->eVelocityX = 0;
            e->switchDirection();
            break;
        default:
            break;
    }
}

void Scene::enemyCollision(){
    for(int i = 0; i < enemies.size(); i++){
        if(player.playerMinX < enemies[i]->enemyMaxX &&
           player.playerMaxX > enemies[i]->enemyMinX &&
           player.playerMinY < enemies[i]->enemyMaxY &&
           player.playerMaxY > enemies[i]->enemyMinY)
        {
            enemyColliderLogic(i);
        }
        else{
            player.colourFlag = 1;
        }
    }
}

void Scene::enemyColliderLogic(int e){
    std::cout << "collided with " << e << std::endl;
    Enemy* enemy = enemies[e];
    int smallest = 0 ;

    distTop = std::abs(player.playerMinY - enemy->enemyMaxY);
    distBot = std::abs(player.playerMaxY - enemy->enemyMinY);
    distRight = std::abs(player.playerMinX - enemy->enemyMaxX);
    distLeft = std::abs(player.playerMaxX - enemy->enemyMinX);

    dists[0] = distTop;
    dists[1] = distBot;
    dists[2] = distRight;
    dists[3] = distLeft;

    for(int i=1;  i < distsSize; i++){
        if(dists[i] < dists[smallest]){
            smallest = i;
        }
    }

    switch(smallest){
        case 0:
            //std::cout << "put on top" << std::endl;
            player.pcY = enemy->enemyMaxY;
            player.pcVelocityY = 0;
            player.pcVelocityY += enemyHitSpeedY;
            player.grounded = false;
            enemies.erase(enemies.begin() + e);
            break;
        case 1:
            //std::cout << "put on bot" << std::endl;
            player.pcY = enemy->enemyMinY - player.pcHeight;
            player.pcVelocityY = 0;
            player.pcVelocityY -= enemyHitSpeedY;
            player.grounded = false;
            if(player.damaged()){
                std::cout << "dead" << std::endl;
                playerAlive = false;
            }
            break;
        case 2:
            //std::cout << "put on right" << std::endl;
            player.pcX = enemy->enemyMaxX;
            player.pcVelocityX = 0;
            if(player.grounded) {
                player.pcVelocityY += enemyHitSpeedY;
                player.pcVelocityX += enemyHitSpeedX;
                player.grounded = false;
            }
            enemy->switchDirection();
            if(player.damaged()){
                std::cout << "dead" << std::endl;
                playerAlive = false;
            }
            break;
        case 3:
            //std::cout << "put on left" << std::endl;
            player.pcX = enemy->enemyMinX - player.pcWidth;
            player.pcVelocityX = 0;
            if(player.grounded){
                player.pcVelocityY += enemyHitSpeedY;
                player.pcVelocityX -= enemyHitSpeedX;
                player.grounded = false;
            }
            enemy->switchDirection();
            if(player.damaged()){
                std::cout << "dead" << std::endl;
                playerAlive = false;
            }
            break;
        default:
            break;
    }
}