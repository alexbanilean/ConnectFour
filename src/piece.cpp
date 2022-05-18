#include "piece.h"

Piece::Piece(){
	circle.setRadius(35);
	circle.setOrigin(Vector2f(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2));
	circle.setPosition(Vector2f(0, 0));
	circle.setFillColor(Color::White);
}

Piece::Piece(Vector2f position, const float radius, Color color){
	circle.setRadius(radius);
	circle.setOrigin(Vector2f(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2));
	circle.setPosition(position);
	circle.setFillColor(color);
}

CircleShape Piece::getShape(){
	return circle;
}

Color Piece::getColor(){
	return circle.getFillColor();
}

void Piece::setPosition(Vector2f newPosition){
	circle.setPosition(newPosition);
}

void Piece::setColor(Color newColor){
	circle.setFillColor(newColor);
}