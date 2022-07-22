#include"King.h"

King::King() {


}
King::King(int player) : Piece(player) {
	if (player == 1) Piece::setCode('k');
	else Piece::setCode('K');
}
King::~King() {

}

vector<Position> King::findPath(Position start, Position dest) {
	vector<Position> v;
	Position direction;

	if (dest.x == start.x && abs (dest.y - start.y) == 1) {
		if (dest.y < start.y) direction.setPosition(0, -1);
		else direction.setPosition(0, 1);

		v.push_back(direction);
	}
	else if (dest.y == start.y && abs (dest.x - start.x) == 1) {

		if (dest.x < start.x) direction.setPosition(-1, 0);
		else direction.setPosition(1, 0);

		v.push_back(direction);
	}
	return v;
}