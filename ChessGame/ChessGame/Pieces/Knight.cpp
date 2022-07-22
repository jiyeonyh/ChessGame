#include"Knight.h"
Knight::Knight(){

}
Knight::Knight(int player) : Piece(player) {
	if (player == 1) Piece::setCode('n');
	else Piece::setCode('N');

}
Knight::~Knight() {

}

vector<Position> Knight::findPath(Position start, Position dest) {
	vector<Position> v;
	Position res = dest - start;

	if (abs(res.x) == 2 && abs(res.y) == 1) v.push_back(res);
	if (abs(res.x) == 1 && abs(res.y) == 2) v.push_back(res);

	return v;
}