#include "Pawn.h"
Pawn::Pawn(){
	_moveState = false;
}
Pawn::Pawn(int player) : Piece(player){
	if (player == 1) Piece::setCode('p');
	else Piece::setCode('P');
	
	_moveState = false;
}
Pawn::~Pawn(){

}

vector<Position> Pawn::findPath(Position start, Position dest) {
	cout << _moveState << endl;
	//목적지까지 이동 경로 찾기 
	vector<Position> v;
	if (dest.y == start.y) {
		if (this->getPlayer() == 1) {
			if (dest.x - start.x == 1) {
				v.push_back(Position(1, 0));
			}
			else if (dest.x - start.x == 2 && _moveState == false) {
				v.push_back(Position(1, 0));
				v.push_back(Position(1, 0));
			}
		}
		else {
			if (dest.x - start.x == -1) {
				v.push_back(Position(-1, 0));
			}
			else if (dest.x - start.x == -2 && _moveState == false) {
				v.push_back(Position(-1, 0));
				v.push_back(Position(-1, 0));
			}
		}
	}

	return v; //이동 불가
}

//상대편 기물 대각선에 있을 때 잡기 
bool Pawn::attack(Position start, Position dest) {
	int player = this->getPlayer();
	int dif = 0;

	if (player == 1) dif = 1;
	else dif = -1;
	
	if (dest.x - start.x == dif && abs(dest.y - start.y) == 1) return true;

	return false;
}

void Pawn::setMoveState(bool state) {
	_moveState = state;
}