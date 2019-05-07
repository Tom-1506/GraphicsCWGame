#include "Utility.h"
#include "Platform.h"
#include "Player.h"
#include "Feather.h"
#include "Enemy.h"
#include "Button.h"

#ifndef GRAPHICSCWGAME_SCENE_H
#define GRAPHICSCWGAME_SCENE_H


class Scene{
    public:
        Scene();

        std::vector<Feather> feathers;
        std::vector<Enemy*> enemies;
        std::vector<Platform*> movingPlatforms;
        std::vector<Button*> buttons;

        std::vector<GLuint> playerWalk;
        std::vector<GLuint> playerJump;

        std::vector<GLuint> enemyWalk;

        bool started;

        void sceneInit();
        void sceneUpdate();
        void sceneCollisions();
        void sceneColliderLogic(int p);
        void movingSceneCollisions();
        void movingSceneColliderLogic(int p);

        void loadButtons();
        void buttonCollision(int x, int y);

        void displayMenu();

        void displayPlayer();

        void loadFeathers();
        void featherCollision();
        void featherColliderLogic(int f);

        void loadEnemies();
        void displayEnemies();
        void enemySceneCollisions();
        void enemySceneColliderLogic(Enemy* e, int p);
        void enemyCollision();
        void enemyColliderLogic(int e);

        void loadMovingPlatforms();
        void displayMovingPlatforms();
        void displayPlatforms();
        void displayFeathers();
};



#endif //GRAPHICSCWGAME_SCENE_H
