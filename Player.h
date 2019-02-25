#include "Utility.h"

#ifndef GRAPHICSCWGAME_PLAYER_H
#define GRAPHICSCWGAME_PLAYER_H


class Player{
    public:
        Player(GLfloat x, GLfloat y);

        void playerInit();
        void playerUpdate();
        void moveUpdate();
        void playerColliderUpdate();
        void controlUpdate();

        float colourFlag;
        bool grounded;

        GLfloat pcX; //player character X position
        GLfloat pcY; //pc Y position
        GLfloat pcWidth; //pc width
        GLfloat pcHeight; //pc height
        GLfloat pcTransX; //pc X translate
        GLfloat pcTransY; //pc Y translate
        float pcVelocityX; //pc movement speed
        float pcVelocityY;
        float gravity; //world gravity
        float playerMaxX;
        float playerMinX;
        float playerMaxY;
        float playerMinY;
};

void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void keySpecialPressed (int key, int x, int y);
void keySpecialUp (int key, int x, int y);

#endif //GRAPHICSCWGAME_PLAYER_H
