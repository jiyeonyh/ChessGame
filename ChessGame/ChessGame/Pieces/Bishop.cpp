#include "Bishop.h"

Bishop::Bishop(){

}

Bishop::Bishop(int player) : Piece(player) {
	if (player == 1) Piece::setCode('b');
	else Piece::setCode('B');
}

Bishop::~Bishop(){

}

vector<Position> Bishop::findPath(Position start, Position dest) {
	vector<Position> v;
	Position res = dest - start;
	Position direction;	// ¹æÇâ

	if (abs(res.x) == abs(res.y)) {
		if (res.x == res.y) {
			if (res.x > 0) 
				direction.setPosition(1, 1);		
			else 
				direction.setPosition(-1, -1);		
				
		}
		else if(res.x < res.y) 
			direction.setPosition(-1, 1);			
		else 
			direction.setPosition(1, -1);			

		for(int i = 0; i < abs(res.x); i++)
			v.push_back(direction);
	}

	return v;
}