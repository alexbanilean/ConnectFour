#include "gamescreen.h"

GameScreen::GameScreen(){
	resolution = Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT);
	window.create(VideoMode(resolution.x, resolution.y), "ConnectFour", Style::Default);
	window.setFramerateLimit(FPS);
	
	StartLayout();
}

void GameScreen::StartLayout(){

	currentGameState = GameState::INITIAL;
	currentGameMode = GameMode::HARD;

	window.clear(backgroundColor);

	Font font;
	if(font.loadFromFile("../assets/arial.ttf")){
		Text title, startTextEasy, startTextHard, instrText;

		title.setFont(font);
		title.setString("Connect Four");
		title.setCharacterSize(40);
		title.setFillColor(displayColor);
		title.setStyle(Text::Bold);
		title.setOrigin(Vector2f(title.getGlobalBounds().width / 2, title.getGlobalBounds().height / 2));
		title.setPosition(600, 150);

		startTextEasy.setFont(font);
		startTextEasy.setString("Press E to start the game in easy mode");
		startTextEasy.setCharacterSize(30);
		startTextEasy.setFillColor(displayColor);
		startTextEasy.setStyle(Text::Bold);
		startTextEasy.setOrigin(Vector2f(startTextEasy.getGlobalBounds().width / 2, startTextEasy.getGlobalBounds().height / 2));
		startTextEasy.setPosition(600, 350);

		startTextHard.setFont(font);
		startTextHard.setString("Press H to start the game in hard mode");
		startTextHard.setCharacterSize(30);
		startTextHard.setFillColor(displayColor);
		startTextHard.setStyle(Text::Bold);
		startTextHard.setOrigin(Vector2f(startTextHard.getGlobalBounds().width / 2, startTextHard.getGlobalBounds().height / 2));
		startTextHard.setPosition(600, 425);

		instrText.setFont(font);
		instrText.setString("Use keys from 1 to 7 to play");
		instrText.setCharacterSize(30);
		instrText.setFillColor(displayColor);
		instrText.setStyle(Text::Bold);
		instrText.setOrigin(Vector2f(instrText.getGlobalBounds().width / 2, instrText.getGlobalBounds().height / 2));
		instrText.setPosition(600, 500);

		window.draw(title);
		window.draw(startTextEasy);
		window.draw(startTextHard);
		window.draw(instrText);
	}

	window.display();
}

void GameScreen::StartTheGame(){

	currentGameState = GameState::RUNNING;

	nr_of_moves = 0;

	float pos_x, pos_y;

	for(int i = 1; i <= 6; ++i)
		for(int j = 1; j <= 7; ++j){
			pos_x =  x_offset + (2 * j - 1) * piece_radius + (j - 1) * x_pieces_gap;
			pos_y = y_offset + (2 * i - 1) * piece_radius + (i - 1) * y_pieces_gap;
			pieces.push_back(Piece(Vector2f(pos_x, pos_y), piece_radius, Color::White));
		}

	for(int i = 0; i < 42; ++i)
		current_pieces[i] = false;

	player_turn = true;
}

void GameScreen::updateMove(int column){

	--column;

	for(int i = 5; i >= 0; --i)
		if(!current_pieces[7 * i + column]){
			++nr_of_moves;
			pieces[7 * i + column].setColor((player_turn ? displayColor : defaultPieceColor));
			current_pieces[7 * i + column] = true;

			check();
			player_turn = !player_turn; // call after check() to verify properly the gameState

			return;
		}
}

void GameScreen::input(){
	Event event{};

	while(window.pollEvent(event)){
		// Window closed
		if(event.type == Event::Closed){
			window.close();
		}

		// Keyboard input
		if(event.type == Event::KeyPressed){
			if(currentGameState == GameState::INITIAL){
				// Quit
				if(Keyboard::isKeyPressed(Keyboard::Escape))
					window.close();

				// Start
				if(event.key.code == Keyboard::E){
					currentGameMode = GameMode::EASY;
					StartTheGame();
				}
				else if(event.key.code == Keyboard::H){
					currentGameMode = GameMode::HARD;
					StartTheGame();
				}
			}
			else if(currentGameState == GameState::RUNNING){
				// Go back to menu
				if(Keyboard::isKeyPressed(Keyboard::Escape))
					currentGameState = GameState::INITIAL;

				// Keys
				if(event.key.code == Keyboard::Num1 || event.key.code == Keyboard::Numpad1)
					updateMove(1);
				else if(event.key.code == Keyboard::Num2 || event.key.code == Keyboard::Numpad2)
					updateMove(2);
				else if(event.key.code == Keyboard::Num3 || event.key.code == Keyboard::Numpad3)
					updateMove(3);
				else if(event.key.code == Keyboard::Num4 || event.key.code == Keyboard::Numpad4)
					updateMove(4);
				else if(event.key.code == Keyboard::Num5 || event.key.code == Keyboard::Numpad5)
					updateMove(5);
				else if(event.key.code == Keyboard::Num6 || event.key.code == Keyboard::Numpad6)
					updateMove(6);
				else if(event.key.code == Keyboard::Num7 || event.key.code == Keyboard::Numpad7)
					updateMove(7);

			}
			else{
				// Go back to menu
				if(Keyboard::isKeyPressed(Keyboard::Escape))
					currentGameState = GameState::INITIAL;

				// Start
				if(event.key.code == Keyboard::Return)
					StartTheGame();
			}
		}
	}
}

int GameScreen::heuristicFunction(int good_positions, int bad_positions, int empty_positions){
	if(good_positions == 4)
		return 1000005; // offensive, go for the winning move
	else if(good_positions == 3 && empty_positions == 1)
		return 10000;
	else if(good_positions == 2 && empty_positions == 2)
		return 1000;
	else if(bad_positions == 2 && empty_positions == 2)
		return -1005; // defensive, block multiple pieces in a row
	else if(bad_positions == 3 && empty_positions == 1)
		return -10005; // defensive, block multiple pieces in a row
	else if(bad_positions == 4)
		return -1000000;

	return 0;
}

int GameScreen::getScore(vector<Color> colors, Color checkColor){
	int good_positions = 0, bad_positions = 0, empty_positions = 0;

	for(auto x: colors)
		if(x == checkColor)
			++good_positions;
		else if(x == Color::White) // empty position color
			++empty_positions;
		else
			++bad_positions;

	return heuristicFunction(good_positions, bad_positions, empty_positions);
}

int GameScreen::heuristicEvaluation(){

	// always called by AI, so check color is defaultPieceColor in getScore
	int score = 0;

	// Line

	for(int i = 0; i <= 5; ++i)
		for(int j = 0; j + 3 <= 6; ++j){
			vector<Color> colors;

			for(int t = 0; t < 4; ++t)
				colors.push_back(pieces[7 * i + j + t].getColor());

			score += getScore(colors, defaultPieceColor);
		}

	// Column

	for(int j = 0; j <= 6; ++j)
		for(int i = 0; i + 3 <= 5; ++i){
			vector<Color> colors;

			for(int t = 0; t < 4; ++t)
				colors.push_back(pieces[7 * (i + t) + j].getColor());

			score += getScore(colors, defaultPieceColor);
		}

	/// Diagonals

	// Main diagonal parallels

	for(int i = 0; i <= 5; ++i){
		int j = 0;
		for(int k = 0; i + k + 3 < 6 && j + k + 3 < 7; ++k){
			vector<Color> colors;

			for(int t = 0; t < 4; ++t)
				colors.push_back(pieces[7 * (i + k + t) + (j + k + t)].getColor());

			score += getScore(colors, defaultPieceColor);
		}
	}

	for(int j = 1; j <= 6; ++j){
		int i = 0;
		for(int k = 0; i + k + 3 < 6 && j + k + 3 < 7; ++k){
			vector<Color> colors;

			for(int t = 0; t < 4; ++t)
				colors.push_back(pieces[7 * (i + k + t) + (j + k + t)].getColor());

			score += getScore(colors, defaultPieceColor);
		}
	}

	// Secondary diagonal parallels

	for(int i = 0; i <= 5; ++i){
		int j = 0;
		for(int k = 0; i - k - 3 >= 0 && j + k + 3 < 7; ++k){
			vector<Color> colors;

			for(int t = 0; t < 4; ++t)
				colors.push_back(pieces[7 * (i - k - t) + (j + k + t)].getColor());

			score += getScore(colors, defaultPieceColor);
		}
	}

	for(int j = 1; j <= 6; ++j){
		int i = 5;
		for(int k = 0; i - k - 3 >= 0 && j + k + 3 < 7; ++k){
			vector<Color> colors;

			for(int t = 0; t < 4; ++t)
				colors.push_back(pieces[7 * (i - k - t) + (j + k + t)].getColor());

			score += getScore(colors, defaultPieceColor);
		}
	}

	return score;
}

pair<int, int> GameScreen::miniMax(int depth, int alpha, int beta, bool maximizingPlayer){
	// returns {bestScore, column}

	if(depth == 0 || (depth >= 42 - nr_of_moves))
		return {heuristicEvaluation(), -1}; // column -1, would be changed up to the tree root

	if(maximizingPlayer){ // AI
		int maxEval = INT_MIN, maxColumn = -1;

		for(int j = 0; j < 7; ++j){
			if(!current_pieces[j]){ // at least a piece can be added on column
				// make move
				for(int i = 5; i >= 0; --i)
					if(!current_pieces[7 * i + j]){
						++nr_of_moves;
						pieces[7 * i + j].setColor(defaultPieceColor);
						current_pieces[7 * i + j] = true;
						break;
					}

				// move made, go further
				int score = miniMax(depth - 1, alpha, beta, !maximizingPlayer).first;

				// discard move
				for(int i = 0; i < 6; ++i)
					if(current_pieces[7 * i + j]){
						--nr_of_moves;
						pieces[7 * i + j].setColor(Color::White);
						current_pieces[7 * i + j] = false;
						break;
					}

				// evaluate
				if(score > maxEval){
					maxEval = score;
					maxColumn = j;
				}

				alpha = max(alpha, score);
				if(beta <= alpha)
					break;
			}
		}

		return {maxEval, maxColumn + 1};
	}
	else{ // Player
		int minEval = INT_MAX, minColumn = -1;

		for(int j = 0; j < 7; ++j){
			if(!current_pieces[j]){ // at least a piece can be added on column
				// make move
				for(int i = 5; i >= 0; --i)
					if(!current_pieces[7 * i + j]){
						++nr_of_moves;
						pieces[7 * i + j].setColor(displayColor);
						current_pieces[7 * i + j] = true;
						break;
					}

				// move made, go further
				int score = miniMax(depth - 1, alpha, beta, !maximizingPlayer).first;

				// discard move
				for(int i = 0; i < 6; ++i)
					if(current_pieces[7 * i + j]){
						--nr_of_moves;
						pieces[7 * i + j].setColor(Color::White);
						current_pieces[7 * i + j] = false;
						break;
					}

				// evaluate

				if(score < minEval){
					minEval = score;
					minColumn = j;
				}

				beta = min(beta, score);
				if(beta <= alpha)
					break;
			}
		}

		return {minEval, minColumn + 1};
	}
}

void GameScreen::aiMove(){

	if(currentGameMode == GameMode::HARD){
		int column = miniMax(MAX_DEPTH, INT_MIN, INT_MAX, player_turn).second;
		updateMove(column);
	}
	else{
		// random column pick
		mt19937 generator(time(0));
		updateMove(generator() % 7 + 1);
	}
}

bool GameScreen::checkWin(Color color){

	// Line

	for(int i = 0; i <= 5; ++i)
		for(int j = 0; j + 3 <= 6; ++j){
			int cnt_same_color = 0;

			for(int t = 0; t < 4; ++t){
				if(current_pieces[7 * i + j + t]){
					if(pieces[7 * i + j + t].getColor() == color)
						++cnt_same_color;
				}
			}

			if(cnt_same_color == 4)
				return true;
		}

	// Column

	for(int j = 0; j <= 6; ++j)
		for(int i = 0; i + 3 <= 5; ++i){

			int cnt_same_color = 0;

			for(int t = 0; t < 4; ++t){
				if(current_pieces[7 * (i + t) + j]){
					if(pieces[7 * (i + t) + j].getColor() == color)
						++cnt_same_color;
				}
			}

			if(cnt_same_color == 4)
				return true;
		}

	/// Diagonals

	// Main diagonal parallels

	for(int i = 0; i <= 5; ++i){
		int j = 0;
		for(int k = 0; i + k + 3 < 6 && j + k + 3 < 7; ++k){

			int cnt_same_color = 0;

			for(int t = 0; t < 4; ++t){
				if(current_pieces[7 * (i + k + t) + (j + k + t)]){
					if(pieces[7 * (i + k + t) + (j + k + t)].getColor() == color)
						++cnt_same_color;
				}
			}

			if(cnt_same_color == 4)
				return true;
		}
	}

	for(int j = 1; j <= 6; ++j){
		int i = 0;
		for(int k = 0; i + k + 3 < 6 && j + k + 3 < 7; ++k){

			int cnt_same_color = 0;

			for(int t = 0; t < 4; ++t){
				if(current_pieces[7 * (i + k + t) + (j + k + t)]){
					if(pieces[7 * (i + k + t) + (j + k + t)].getColor() == color)
						++cnt_same_color;
				}
			}

			if(cnt_same_color == 4)
				return true;
		}
	}

	// Secondary diagonal parallels

	for(int i = 0; i <= 5; ++i){
		int j = 0;
		for(int k = 0; i - k - 3 >= 0 && j + k + 3 < 7; ++k){

			int cnt_same_color = 0;

			for(int t = 0; t < 4; ++t){
				if(current_pieces[7 * (i - k - t) + (j + k + t)]){
					if(pieces[7 * (i - k - t) + (j + k + t)].getColor() == color)
						++cnt_same_color;
				}
			}

			if(cnt_same_color == 4)
				return true;
		}
	}

	for(int j = 1; j <= 6; ++j){
		int i = 5;
		for(int k = 0; i - k - 3 >= 0 && j + k + 3 < 7; ++k){

			int cnt_same_color = 0;

			for(int t = 0; t < 4; ++t){
				if(current_pieces[7 * (i - k - t) + (j + k + t)]){
					if(pieces[7 * (i - k - t) + (j + k + t)].getColor() == color)
						++cnt_same_color;
				}
			}

			if(cnt_same_color == 4)
				return true;
		}
	}

	return false;
}

void GameScreen::check(){

	if(nr_of_moves == 42){
		currentGameState = GameState::OVER;
		result = Result::DRAW;
		return;
	}
	
	if(player_turn){
		if(checkWin(displayColor)){
			currentGameState = GameState::OVER;
			result = Result::WIN;
		}
	}
	else{
		if(checkWin(defaultPieceColor)){
			currentGameState = GameState::OVER;
			result = Result::LOSS;
		}
	}
}

void GameScreen::draw(){

	window.clear(backgroundColor);

	Texture img;
	img.loadFromFile("../assets/Connect4Table.png");
	Sprite s(img);
	s.setOrigin(Vector2f(s.getGlobalBounds().width / 2, s.getGlobalBounds().height / 2));
	s.setPosition(600, 450);

	window.draw(s);

	Font font;
	if(font.loadFromFile("../assets/arial.ttf")){
		Text title;

		title.setFont(font);
		title.setString("Connect Four");
		title.setCharacterSize(40);
		title.setFillColor(displayColor);
		title.setStyle(Text::Bold);
		title.setOrigin(Vector2f(title.getGlobalBounds().width / 2, title.getGlobalBounds().height / 2));
		title.setPosition(600, 100);

		window.draw(title);
	}

	for(int i = 0; i <= 5; ++i)
		for(int j = 0; j <= 6; ++j)
			if(current_pieces[7 * i + j])
				window.draw(pieces[7 * i + j].getShape());
	
	if(currentGameState == GameState::OVER){
		if(font.loadFromFile("../assets/arial.ttf")){
			Text overText, instr;

			overText.setFont(font);
			
			if(result == Result::WIN){
				overText.setString("WIN");
				overText.setFillColor(Color::Green);
			}
			else if(result == Result::LOSS){
				overText.setString("LOSS");
				overText.setFillColor(Color::Red);
			}
			else{
				overText.setString("DRAW");
				overText.setFillColor(Color::Yellow);
			}
			
			overText.setCharacterSize(60);
			overText.setStyle(Text::Bold);
			overText.setOrigin(Vector2f(overText.getGlobalBounds().width / 2, overText.getGlobalBounds().height / 2));
			overText.setPosition(600, 300);

			instr.setFont(font);
			instr.setString("Press ENTER to play again");
			instr.setCharacterSize(60);
			instr.setFillColor(overText.getFillColor());
			instr.setStyle(Text::Bold);
			instr.setOrigin(Vector2f(instr.getGlobalBounds().width / 2, instr.getGlobalBounds().height / 2));
			instr.setPosition(600, 400);
			
			window.draw(instr);
			window.draw(overText);
		}
	}

	window.display();
}