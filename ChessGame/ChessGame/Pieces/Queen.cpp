#include"Queen.h"

Queen::Queen() {

}
Queen::Queen(int player) : Piece(player){
	if (player == 1) Piece::setCode('q');
	else Piece::setCode('Q');
}
Queen::~Queen() {

}

vector<Position> Queen::findPath(Position start, Position dest) {
	vector<Position> v;
	Position res = dest - start;
	Position direction;	// 방향

	//앞, 뒤, 양옆으로 이동
	if (dest.x == start.x) {

		if (dest.y < start.y) 
			direction.setPosition(0, -1);
		else 
			direction.setPosition(0, 1);

		for (int i = 0; i < abs(dest.y - start.y); i++)
			v.push_back(direction);
	}
	else if (dest.y == start.y) {

		if (dest.x < start.x) 
			direction.setPosition(-1, 0);
		else
			direction.setPosition(1, 0);

		for (int i = 0; i < abs(dest.x - start.x); i++)
			v.push_back(direction);
	}

	//대각선으로 이동
	if (abs(res.x) == abs(res.y)) {
		if (res.x == res.y) {
			if (res.x > 0)
				direction.setPosition(1, 1);		
			else
				direction.setPosition(-1, -1);		

		}
		else if (res.x < res.y)
			direction.setPosition(-1, 1);			
		else
			direction.setPosition(1, -1);			

		for (int i = 0; i < abs(res.x); i++)
			v.push_back(direction);
	}
	return v;
}