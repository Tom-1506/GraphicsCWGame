#include "Utility.h"
#include "Player.h"

#ifndef GRAPHICSCWGAME_SCENE_H
#define GRAPHICSCWGAME_SCENE_H


class Scene{
    public:
        Scene();

        void sceneInit();
        void sceneUpdate();
        void sceneColliderLogic(int p);
        void sceneCollisions();
};

void renderPlatforms();

#endif //GRAPHICSCWGAME_SCENE_H
