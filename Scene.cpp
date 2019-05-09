#include "Scene.h"

GLuint title;
GLuint play;
GLuint restart;
GLuint quit;
GLuint playerTex; //player texture
GLuint ground; //ground texture
GLuint feather; //feather texture
GLuint goldFeather;
GLuint background; //background texture
GLuint message;
GLuint health;
GLuint died;

int featherCountDis;

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
int healthBlockW = 270;

bool playerAlive = true;
Player player = Player(1200, 1200);

Platform platforms[] = {Platform(1028, 768, 9*blockWidth, blockHeight, 9, 1, false, 0, 0),
                        Platform(-1624, 1700, 6*blockWidth, blockHeight, 6, 1, false, 0, 0),
                        Platform(-1496, 1828, 2*blockWidth, blockHeight, 2, 1, false, 0, 0)};
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
    started = false;
    player.numFeathers = 0;
    player.health = 5;
    player.pcVelocityX = 0;
    player.pcVelocityY = 0;
    player.pcX = 1200;
    player.pcY = 1200;
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
    title = loadPNG((char*)"textures/title.png");
    play = loadPNG((char*)"textures/play-button.png");
    restart = loadPNG((char*)"textures/restart-button.png");
    quit = loadPNG((char*)"textures/quit-button.png");
    feather = loadPNG((char*)"textures/feather.png");
    goldFeather = loadPNG((char*)"textures/gold-feather.png");
    for(int i = 0; i < 6; i++){
        std::string fpath = "textures/numbers/" + std::to_string(i) + ".png";
        char cstr[fpath.size() + 1];
        strcpy(cstr, fpath.c_str());
        featherCounter.emplace_back(loadPNG(cstr));
    }
    message = loadPNG((char*)"textures/message.png");
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

        //Objective message
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, message);
                glColor3f(1, 0, 1);
                drawQuad(0, 1000, 1040, 712, 1, 1);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        for(Platform p : platforms){
            p.platformUpdate();
        }

        for(Platform* m : movingPlatforms){
            m->platformUpdate();
        }

        for(int i = 0; i < enemies.size(); i++){
            enemies[i]->enemyUpdate();
        }

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
        displayEnemies();
        displayFeathers();

        //health bar
        glLoadIdentity();
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, health);
                glColor3f(0, 1, 0);
                drawQuad(200, 1900, healthBlockW*player.health, 50, player.health, 1);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //feather counter
        if(playerAlive){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                    glBindTexture(GL_TEXTURE_2D, feather);
                    glColor3f(1, 0, 1);
                    drawQuad(100, 1850, 148, 148, 1, 1);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                    glBindTexture(GL_TEXTURE_2D, featherCounter[player.numFeathers]);
                    glColor3f(1, 0, 1);
                    drawQuad(100, 1875, 97, 129, 1, 1);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }

        glTranslatef(-player.pcX+1740, -player.pcY+952, 0);

        //dead
        if(!playerAlive){
            glLoadIdentity();
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                    glBindTexture(GL_TEXTURE_2D, died);
                    glColor3f(0, 1, 0);
                    drawQuad(1100, 1200, 1408, 364, 1, 1);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            for(Button* b : endButtons){
                glPushMatrix();
                    glEnable(GL_TEXTURE_2D);
                        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                        if(b->bY == 800){
                            glBindTexture(GL_TEXTURE_2D, restart);
                        }
                        else{
                            glBindTexture(GL_TEXTURE_2D, quit);
                        }
                        glColor3f(0, 1, 0);
                        drawQuad(b->bX, b->bY, b->buttonWidth, b->buttonHeight, b->buttonTexX, b->buttonTexY);
                    glDisable(GL_TEXTURE_2D);
                glPopMatrix();
            }
        }
    }
    else if(won){
        //Won menu
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, background);
                glColor3f(0, 1, 0);
                drawQuad(0, 0, worldWidth, worldHeight, 3, 2);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        displayWinMenu();
    }
    else{
        //menu background
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, background);
                glColor3f(0, 1, 0);
                drawQuad(0, 0, worldWidth, worldHeight, 3, 2);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        displayMenu();
    }
}

void Scene::loadButtons(){
    startButtons.clear();
    endButtons.clear();
    startButtons.emplace_back(new Button(1450, 800, 700, 200, 1, 1));
    startButtons.emplace_back(new Button(1450, 450, 700, 200, 1, 1));
    endButtons.emplace_back(new Button(1450, 800, 700, 200, 1, 1));
    endButtons.emplace_back(new Button(1450, 450, 700, 200, 1, 1));
}

void Scene::displayMenu(){
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, title);
            glColor3f(0, 1, 0);
            drawQuad(900, 1200, 1800, 800, 1, 1);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    for(Button* b : startButtons){
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                if(b->bY == 800){
                    glBindTexture(GL_TEXTURE_2D, play);
                }
                else{
                    glBindTexture(GL_TEXTURE_2D, quit);
                }
                glColor3f(0, 1, 0);
                drawQuad(b->bX, b->bY, b->buttonWidth, b->buttonHeight, b->buttonTexX, b->buttonTexY);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void Scene::displayWinMenu(){
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, title);
            glColor3f(0, 1, 0);
            drawQuad(900, 1200, 1800, 800, 1, 1);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    for(Button* b : endButtons){
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                if(b->bY == 800){
                    glBindTexture(GL_TEXTURE_2D, restart);
                }
                else{
                    glBindTexture(GL_TEXTURE_2D, quit);
                }
                glColor3f(0, 1, 0);
                drawQuad(b->bX, b->bY, b->buttonWidth, b->buttonHeight, b->buttonTexX, b->buttonTexY);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void Scene::buttonCollision(int x, int y){
    if(!started){
        Button* play = startButtons[0];
        Button* quit = startButtons[1];
        if(x > play->buttonMinX &&
           x < play->buttonMaxX &&
           y > play->buttonMinY &&
           y < play->buttonMaxY && !started){
            started = true;
        }
        else if(x > quit->buttonMinX &&
                x < quit->buttonMaxX &&
                y > quit->buttonMinY &&
                y < quit->buttonMaxY){
            exit(0);
        }
    }
    else if(won || !playerAlive){
        Button* restart = endButtons[0];
        Button* quit = endButtons[1];
        if(x > restart->buttonMinX &&
           x < restart->buttonMaxX &&
           y > restart->buttonMinY &&
           y < restart->buttonMaxY){
            std::cout << "here" << std::endl;
            sceneInit();
            started = true;
            playerAlive = true;
            won = false;
        }
        else if(x > quit->buttonMinX &&
                x < quit->buttonMaxX &&
                y > quit->buttonMinY &&
                y < quit->buttonMaxY){
            exit(0);
        }
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
    for(int i = 0; i < feathers.size(); i++){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                if(i == 0){
                    glBindTexture(GL_TEXTURE_2D, goldFeather);
                }
                else{
                    glBindTexture(GL_TEXTURE_2D, feather);
                }
                glColor3f(1, 0, 1);
                drawQuad(feathers[i].fX, feathers[i].fY, feathers[i].fWidth, feathers[i].fHeight, 1, 1);
            glDisable(GL_TEXTURE_2D);
            glLineWidth(15);
            glColor3f(1, player.colourFlag, player.colourFlag);
            if(drawCollisionBoxes){
                drawBox(feathers[i].fX, feathers[i].fY, feathers[i].fWidth, feathers[i].fHeight);
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
    movingPlatforms.clear();
    movingPlatforms.emplace_back(new Platform(400, 0, 2*blockWidth, blockHeight, 2, 1, true, 800, 0.5));

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
    feathers.clear();
    feathers.emplace_back(Feather(-1300, 2050)); //gold feather
    feathers.emplace_back(Feather(1600, 1100));
    feathers.emplace_back(Feather(2000, 1100));
    feathers.emplace_back(Feather(2400, 1100));
    feathers.emplace_back(Feather(2800, 1100));
    feathers.emplace_back(Feather(3200, 1100));
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
    if(f == 0){ //gold feather
        won = true;
        started = false;
        feathers.erase(feathers.begin()+f);
    }
    else{
        player.colourFlag = 0;
        player.featherGet();
        feathers.erase(feathers.begin()+f);
    }
}

void Scene::loadEnemies(){
    enemies.clear();
    enemies.emplace_back(new Enemy(-900, 2050));
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
                    playerAlive = false;
                }
            }
            break;
        default:
            break;
    }
}