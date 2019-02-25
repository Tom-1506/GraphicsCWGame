#include "Utility.h"

#ifndef GRAPHICSCWGAME_SCENE_H
#define GRAPHICSCWGAME_SCENE_H


class Scene{
    public:
        Scene();

        void sceneInit();
        void sceneUpdate();
        void sceneColliderUpdate();
        void sceneCollisions();

        float groundMaxX;
        float groundMinX;
        float groundMaxY;
        float groundMinY;
};

#endif //GRAPHICSCWGAME_SCENE_H
