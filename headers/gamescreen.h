#ifndef CONNECTFOUR_GAMESCREEN_H
#define CONNECTFOUR_GAMESCREEN_H

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include "piece.h"

using namespace sf;
using namespace std;

class GameScreen {

private:
    // Window
    Vector2f resolution;
    RenderWindow window;
    const unsigned int FPS = 60; // frames per second

    const float WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 800;
	const float x_pieces_gap = 38.3, y_pieces_gap = 20, piece_radius = 35, x_offset = 240, y_offset = 190;
	const int MAX_DEPTH = 5;

	Color backgroundColor = Color(65, 75, 80);
	Color displayColor = Color(90, 200, 230);
	Color defaultPieceColor = Color::Black;

	vector<Piece> pieces = {};
	std::vector<bool> current_pieces = vector(42, false);
	bool player_turn;

	int currentGameState, result, nr_of_moves, currentGameMode;

public:
    enum GameState { RUNNING, INITIAL, OVER };
	enum Result { WIN, LOSS, DRAW };
	enum GameMode { EASY, HARD };

    GameScreen();

    void StartLayout();

    void StartTheGame();

	int heuristicFunction(int good_states, int bad_states, int empty_states);
	int heuristicEvaluation();
	pair<int, int> miniMax(int depth, int alpha, int beta, bool player);

	void updateMove(int column);
	void aiMove();

	void check();

	int getScore(vector<Color> colors, Color checkColor);
	bool checkWin(Color color);

	void input();

    void draw();

    friend class Game;
};

#endif //CONNECTFOUR_GAMESCREEN_H
