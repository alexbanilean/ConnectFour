#ifndef CONNECTFOUR_PIECE_H
#define CONNECTFOUR_PIECE_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Piece{

private:
	CircleShape circle;

public:

	Piece();

	Piece(Vector2f position, const float radius, Color color);

	CircleShape getShape();

	Color getColor();

	void setPosition(Vector2f newPosition);

	void setColor(Color newColor);
};


#endif //CONNECTFOUR_PIECE_H
