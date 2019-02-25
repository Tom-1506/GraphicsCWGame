#include "Utility.h"

#ifndef GRAPHICSCWGAME_PLAYER_H
#define GRAPHICSCWGAME_PLAYER_H


class Player{
    public:
        Player(GLfloat x, GLfloat y);

        void playerInit();
        void playerUpdate();
        void controlUpdate();

        //Variable definitions
        GLfloat pcX; //player character X position
        GLfloat pcY; //pc Y position
        GLfloat pcWidth; //pc width
        GLfloat pcHeight; //pc height
        GLfloat pcTransX; //pc X translate
        GLfloat pcTransY; //pc Y translate
        float pcSpeed; //pc movement speed
        float gravity; //world gravity
};

void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void keySpecialPressed (int key, int x, int y);
void keySpecialUp (int key, int x, int y);

#endif //GRAPHICSCWGAME_PLAYER_H
