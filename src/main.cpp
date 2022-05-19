#include "../headers/game.h"
#include <windows.h>

int main() {

	ShowWindow(GetConsoleWindow(), SW_HIDE);

    Game ConnectFour;

    ConnectFour.mainLoop();

    return 0;
}
