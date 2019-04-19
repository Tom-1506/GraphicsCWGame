#include "Utility.h"

#ifndef GRAPHICSCWGAME_ENEMY_H
#define GRAPHICSCWGAME_ENEMY_H


class Enemy {
    public:
        Enemy(GLfloat x, GLfloat y);

        void enemyInit();
        void enemyUpdate();
        void enemyDisplay(GLuint enemyTex);
        void moveUpdate();
        void enemyColliderUpdate();

        float colourFlag;
        bool grounded;

        GLfloat eX;
        GLfloat eY;
        GLfloat enemyWidth;
        GLfloat enemyHeight;
        float eVelocityX;
        float eVelocityY;
        double gravity;
        float enemyMaxX;
        float enemyMinX;
        float enemyMaxY;
        float enemyMinY;
};

#endif //GRAPHICSCWGAME_ENEMY_H