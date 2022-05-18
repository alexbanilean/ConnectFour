#include "game.h"

void Game::mainLoop(){
    Clock clock;

    while(screen.window.isOpen()){
        Time time = clock.restart();

		if(screen.currentGameState == GameScreen::GameState::INITIAL){
			screen.StartLayout();
			screen.input();
		}
		else if(screen.currentGameState == GameScreen::GameState::RUNNING){

			screen.check();
			screen.draw();

			if(screen.currentGameState == GameScreen::GameState::RUNNING)
				if(screen.player_turn)
					screen.input();
				else
					screen.aiMove();
		}
		else{
			screen.draw();
			screen.input();
		}
    }
}