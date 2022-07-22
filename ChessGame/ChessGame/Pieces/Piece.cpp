#include "Piece.h"

Piece::Piece() {
}

Piece::Piece(int player) : _player(player){

}

Piece::~Piece() {
}

int Piece::getPlayer() {
	
	return _player;
}

void Piece::setCode(char code) {
	_code = code;
}

char Piece::getCode() {

	return _code;
}