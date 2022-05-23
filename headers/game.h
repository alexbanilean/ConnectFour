#ifndef CONNECTFOUR_GAME_H
#define CONNECTFOUR_GAME_H

#include "gamescreen.h"

class Game {

private:
	GameScreen screen;

public:
	Game(){}

    void mainLoop();
};

#endif //CONNECTFOUR_GAME_H
