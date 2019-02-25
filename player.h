#include "utility.h"

#ifndef GRAPHICSCWGAME_PLAYER_H
#define GRAPHICSCWGAME_PLAYER_H


class player {

};

void playerInit();
void playerUpdate();
void controlUpdate();
void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void keySpecialPressed (int key, int x, int y);
void keySpecialUp (int key, int x, int y);

#endif //GRAPHICSCWGAME_PLAYER_H
