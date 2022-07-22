#include "Rook.h"

Rook::Rook() {

}
Rook::Rook(int player):Piece(player) {
	if (player == 1) Piece::setCode('r');
	else Piece::setCode('R');
}
Rook::~Rook() {

}

vector<Position> Rook::findPath(Position start, Position dest) {
	
	vector<Position> v;
	Position direction;

	if (dest.x == start.x) {
		if (dest.y < start.y) direction.setPosition(0, -1);
		else direction.setPosition(0, 1);

		for (int i = 0; i < abs(dest.y - start.y); i++) 
			v.push_back(direction);
	}
	else if (dest.y == start.y) {

		if (dest.x < start.x) direction.setPosition(-1, 0);
		else direction.setPosition(1, 0);

		for (int i = 0; i < abs(dest.x - start.x); i++)
			v.push_back(direction);
	}

	return v;
}