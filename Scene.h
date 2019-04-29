#include "Utility.h"
#include "Platform.h"
#include "Player.h"
#include "Feather.h"
#include "Enemy.h"

#ifndef GRAPHICSCWGAME_SCENE_H
#define GRAPHICSCWGAME_SCENE_H


class Scene{
    public:
        Scene();

        std::vector<Feather> feathers;
        std::vector<Enemy*> enemies;
        std::vector<Platform*> movingPlatforms;

        void sceneInit();
        void sceneUpdate();
        void sceneCollisions();
        void sceneColliderLogic(int p);
        void movingSceneCollisions();
        void movingSceneColliderLogic(int p);

        void loadFeathers();
        void featherCollision();
        void featherColliderLogic(int f);

        void loadEnemies();
        void enemySceneCollisions();
        void enemySceneColliderLogic(Enemy* e, int p);
        void enemyCollision();
        void enemyColliderLogic(int e);

        void loadMovingPlatforms();
        void renderPlatforms();
        void renderFeathers();
};



#endif //GRAPHICSCWGAME_SCENE_H
