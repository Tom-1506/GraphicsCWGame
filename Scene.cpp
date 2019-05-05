#include "Scene.h"

GLuint playerTex; //player texture
GLuint ground; //ground texture
GLuint feather; //feather texture
GLuint background; //background texture
GLuint health;
GLuint died;

float enemyHitSpeedY = 1;
float enemyHitSpeedX = 1;

int playerWalkTexCounter = 0;
int enemyWalkTexCounter = 0;
int sc;
bool squashEnemy;
int jumpTexCounter = 0;
int texChangeTimer = 0;
int texTimer = 75;

int worldWidth = 7680;
int worldHeight = 4320;
int blockWidth = 256;
int blockHeight = 128;
int healthBlockW = 320;

bool playerAlive = true;
Player player = Player(1200, 1200);

Platform platforms[] = {Platform(3600, 0, 2*blockWidth, blockHeight, 2, 1, false, 0, 0),
                        Platform(1028, 768, 3*blockWidth, blockHeight, 3, 1, false, 0, 0),
                        Platform(2000, blockHeight, 4*blockWidth, blockHeight, 4, 1, false, 0, 0),
                        Platform(3100, 800, blockWidth, blockHeight, 1, 1, false, 0, 0),
                        Platform(2200, 1400, 3*blockWidth, blockHeight, 3, 1, false, 0, 0),
                        Platform(4500, 256, 5*blockWidth, blockHeight, 5, 1, false, 0, 0),
                        Platform(5840, 700, 3*blockWidth, blockHeight, 3, 1, false, 0, 0)};
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
    started = true;
    loadMovingPlatforms();
    player.playerInit();
    loadFeathers();
    loadEnemies();
    loadButtons();
    for(int i = 0; i < enemies.size(); i++){
        enemies[i]->enemyInit();
    }

    playerTex = loadPNG((char*)"textures/soma/idle.png");
    for(int i = 1; i < 18; i++){
        std::string fpath = "textures/soma/walking/walk" + std::to_string(i) + ".png";
        char cstr[fpath.size() + 1];
        strcpy(cstr, fpath.c_str());
        playerWalk.emplace_back(loadPNG(cstr));
    }
    for(int i = 1; i < 7; i++){
        std::string fpath = "textures/soma/jumping/jump" + std::to_string(i) + ".png";
        char cstr[fpath.size() + 1];
        strcpy(cstr, fpath.c_str());
        playerJump.emplace_back(loadPNG(cstr));
    }
    for(int i = 1; i < 17; i++){
        std::string fpath = "textures/armour/walk" + std::to_string(i) + ".png";
        char cstr[fpath.size() + 1];
        strcpy(cstr, fpath.c_str());
        enemyWalk.emplace_back(loadPNG(cstr));
    }
    ground = loadPNG((char*)"textures/blocks.png");
    feather = loadPNG((char*)"textures/feather.png");
    background = loadPNG((char*)"textures/sky-backdrop.png");
    health = loadPNG((char*) "textures/health.png");
    died = loadPNG((char*) "textures/you-died.png");
}

void Scene::sceneUpdate(){
    if(started){
        //background
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, background);
                glColor3f(0, 1, 0);
                drawQuad(0, 0, worldWidth, worldHeight, 3, 2);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glTranslatef(-player.pcX+1740, -player.pcY+952, 0);

        for(Platform p : platforms){
            p.platformUpdate();
        }

        for(Platform* m : movingPlatforms){
            m->platformUpdate();
        }

        //std::cout << "start" << std::endl;
        for(int i = 0; i < enemies.size(); i++){
            enemies[i]->enemyUpdate();
        }
        //std::cout << "end" << std::endl;

        if(playerAlive){
            player.playerUpdate();
            if(player.pcY < -1000){
                player.health = 0;
                playerAlive = false;
            }
        }

        sceneCollisions();
        movingSceneCollisions();
        enemySceneCollisions();
        featherCollision();

        if(playerAlive){
            enemyCollision();
            displayPlayer();
        }

        displayMovingPlatforms();
        displayPlatforms();

        //health bar
        glLoadIdentity();
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, health);
                glColor3f(0, 1, 0);
                drawQuad(100, 1900, healthBlockW*player.health, 80, player.health, 1);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glTranslatef(-player.pcX+1740, -player.pcY+952, 0);

        displayEnemies();
        displayFeathers();

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
    else{
        //render menu here
    }
}

void Scene::displayPlayer(){
    if(playerWalkTexCounter > 16){
        playerWalkTexCounter = 0;
    }
    if(player.grounded){
        jumpTexCounter = 0;
    }

    //Player character
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_CLAMP_TO_BORDER);
            if(player.jumping){
                glBindTexture(GL_TEXTURE_2D, playerJump[jumpTexCounter]);
            }
            else if(player.falling || (player.idle && !player.grounded)){
                glBindTexture(GL_TEXTURE_2D, playerJump[5]);
            }
            else if(player.idle && player.grounded){
                glBindTexture(GL_TEXTURE_2D, playerTex);
            }
            else{
                if(player.movingRight || player.movingLeft){
                    glBindTexture(GL_TEXTURE_2D, playerWalk[playerWalkTexCounter]);
                }
            }
            glTranslatef(player.pcX, player.pcY, 0);
            glColor3f(1, 0, 0);
            if(player.idle && !player.grounded){
                drawQuad(0, 0, player.pcWidth+60, player.pcHeight, 1, 1);
            }
            else if(player.idle){
                drawQuad(0, 0, player.pcWidth, player.pcHeight, 1, 1);
            }
            else if(player.movingRight && !player.grounded){
                drawQuad(0, 0, player.pcWidth+60, player.pcHeight, 1, 1);
            }
            else if(player.movingLeft && !player.grounded){
                drawQuad(0, 0, player.pcWidth+60, player.pcHeight, -1, 1);
            }
            else if(player.movingRight){
                drawQuad(0, 0, player.pcWidth+30, player.pcHeight, 1, 1);
            }
            else if(player.movingLeft){
                drawQuad(0, 0, player.pcWidth+30, player.pcHeight, -1, 1);
            }
        glDisable(GL_TEXTURE_2D);
        glLineWidth(15);
        glColor3f(1, player.colourFlag, player.colourFlag);
        if(drawCollisionBoxes){
            drawBox(0, 0, player.pcWidth, player.pcHeight);
        }
    glPopMatrix();

    texChangeTimer += deltaTime;
    if(texChangeTimer > texTimer){
        playerWalkTexCounter++;
        if(jumpTexCounter < 5){
            jumpTexCounter++;
        }
        texChangeTimer = 0;
    }
}

void Scene::displayEnemies(){
    if(enemyWalkTexCounter > 15){
        enemyWalkTexCounter = 0;
    }

    sc = enemyWalkTexCounter;
    if(sc == 2 || sc == 3 || sc == 4 || sc == 11 || sc == 12 || sc == 13){
        squashEnemy = true;
    }
    else{
        squashEnemy = false;
    }
    //Enemy
    for(Enemy* e : enemies){
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, enemyWalk[enemyWalkTexCounter]);
                glTranslatef(e->eX, e->eY, 0);
                glColor3f(0.5, 0, 1);
                if(e->facingRight && squashEnemy){
                    drawQuad(0, 0, e->enemyWidth-20, e->enemyHeight, -1, 1);
                }
                else if(e->facingRight){
                    drawQuad(0, 0, e->enemyWidth, e->enemyHeight, -1, 1);
                }
                else if(!e->facingRight && squashEnemy){
                    drawQuad(0, 0, e->enemyWidth-20, e->enemyHeight, 1, 1);
                }
                else if(!e->facingRight){
                    drawQuad(0, 0, e->enemyWidth, e->enemyHeight, 1, 1);
                }

            glDisable(GL_TEXTURE_2D);
            glLineWidth(15);
            glColor3f(1, e->colourFlag, e->colourFlag);
            if(drawCollisionBoxes){
                drawBox(0, 0, e->enemyWidth, e->enemyHeight);
            }
        glPopMatrix();
    }

    texChangeTimer += deltaTime;
    if(texChangeTimer > texTimer){
        enemyWalkTexCounter++;
        texChangeTimer = 0;
    }
}

void Scene::displayMovingPlatforms(){
    for(Platform* m : movingPlatforms){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, ground);
                glColor3f(0, 0, 0);
                drawQuad(m->platX, m->platY, m->platWidth, m->platHeight, m->platTexX, m->platTexY);
            glDisable(GL_TEXTURE_2D);
            glLineWidth(15);
            glColor3f(1, player.colourFlag, player.colourFlag);
            if(drawCollisionBoxes){
                drawBox(m->platX, m->platY, m->platWidth, m->platHeight);
            }
        glPopMatrix();
    }
}

void Scene::displayPlatforms(){
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

void Scene::displayFeathers(){
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

void Scene::loadMovingPlatforms(){
    movingPlatforms.emplace_back(new Platform(400, 0, 2*blockWidth, blockHeight, 2, 1, true, 800, 0.5));
    movingPlatforms.emplace_back(new Platform(3500, 850, 3*blockWidth, blockHeight, 3, 1, true, 800, 0.6));
    movingPlatforms.emplace_back(new Platform(300, 1400, 2*blockWidth, blockHeight, 2, 1, true, 1000, 0.6));
    movingPlatforms.emplace_back(new Platform(3000, 2000, blockWidth, blockHeight, 1, 1, true, 500, 0.3));
}

void Scene::movingSceneCollisions(){
    for(int i = 0; i < movingPlatforms.size(); i++){
        Platform* p = movingPlatforms[i];
        if(player.playerMinX < p->platMaxX &&
           player.playerMaxX > p->platMinX &&
           player.playerMinY <= p->platMaxY &&
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
            //std::cout << "scene: " << plat->platVelocityX << " " << deltaTime<< std::endl;
            player.grounded = true;
            player.pcX += (plat->platVelocityX * deltaTime);
            player.pcY = plat->platMaxY;
            if(player.pcVelocityY < 0)
            {
                player.pcVelocityY = 0;
            }
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
    enemies.emplace_back(new Enemy(2100, 300));
    enemies.emplace_back(new Enemy(2300, 1600));
    enemies.emplace_back(new Enemy(2400, 300));
    /*enemies.emplace_back(new Enemy(2300, 300));
    enemies.emplace_back(new Enemy(2500, 1600));*/
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
                if((e->eX + e->enemyWidth*0.75) > platforms[p].platMaxX && e->facingRight){
                    e->switchDirection();
                }
                if((e->eX + e->enemyWidth*0.25) < platforms[p].platMinX && !e->facingRight){
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
            if(!player.hit){
                if(player.damaged()){
                    std::cout << "dead" << std::endl;
                    playerAlive = false;
                }
            }
            break;
        case 2:
            //std::cout << "put on right" << std::endl;
            player.pcX = enemy->enemyMaxX;
            player.pcVelocityX = 0;
            player.pcVelocityX += enemyHitSpeedX;
            player.grounded = false;
            enemy->switchDirection();
            if(!player.hit){
                player.pcVelocityY += enemyHitSpeedY;
                if(player.damaged()){
                    std::cout << "dead" << std::endl;
                    playerAlive = false;
                }
            }
            break;
        case 3:
            //std::cout << "put on left" << std::endl;
            player.pcX = enemy->enemyMinX - player.pcWidth;
            player.pcVelocityX = 0;
            player.pcVelocityX -= enemyHitSpeedX;
            player.grounded = false;
            enemy->switchDirection();
            if(!player.hit){
                player.pcVelocityY += enemyHitSpeedY;
                if(player.damaged()){
                    std::cout << "dead" << std::endl;
                    playerAlive = false;
                }
            }
            break;
        default:
            break;
    }
}

void Scene::loadButtons(){

}