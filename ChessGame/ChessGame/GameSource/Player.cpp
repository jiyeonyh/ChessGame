#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include "Pieces/Pawn.h"
#include "Pieces/Knight.h"
#include "Pieces/King.h"

#define MAX_PIECE 16

Player::Player() {

}

Player::Player(int playerNum, ChessBoard* chessboard) {
	_playerNum = playerNum;
	_chessboard = chessboard;
	_winFlag = false;
}

Player::~Player() {

}

bool Player::selectPiece(Position start) {

	if (_chessboard->_board[start.x][start.y] == NULL) return false;
	if (_playerNum == BLACK && isalpha(_chessboard->_board[start.x][start.y]->getCode())==LOWERCASE) return true;
	else if (_playerNum == WHITE && isalpha(_chessboard->_board[start.x][start.y]->getCode()) == UPPERCASE) return true;

	return false;

}

bool Player::movePiece(Position start, Position dest) {
	Piece* (*board)[8] = _chessboard->_board;

	/* pawn attack */
	if (typeid(*board[start.x][start.y]) == typeid(Pawn) && board[dest.x][dest.y] != NULL) {
		Pawn* p = dynamic_cast<Pawn*>(board[start.x][start.y]);
		if (p->attack(start, dest)) {
			if (!capturePiece(dest)) return false;

			board[dest.x][dest.y] = board[start.x][start.y];
			board[start.x][start.y] = NULL;

			return true;

		}
		return false;
	}

	vector<Position> path = board[start.x][start.y]->findPath(start, dest);
	
	if (!path.empty()) {
		if (typeid(*board[start.x][start.y]) == typeid(Knight) || canMove(start, path)) {
			if (board[dest.x][dest.y] != NULL) {	 //attack
				if (!capturePiece(dest)) return false;
			}
			board[dest.x][dest.y] = board[start.x][start.y];
			board[start.x][start.y] = NULL;

			if (typeid(*board[dest.x][dest.y]) == typeid(Pawn)) {
				Pawn* p = dynamic_cast<Pawn*>(board[dest.x][dest.y]);
				p->setMoveState(true);
			}
			return true;
		}
	}

	return false;
}

//경로 따라가면서 이동할 수 있는 지(장애물 없는지) 확인
bool Player::canMove(Position start, const vector<Position>& path) {
	Piece* (*board)[8] = _chessboard->_board;

	int next_x = start.x;
	int next_y = start.y;

	for (int i = 0; i < path.size(); i++) {
		next_x += path[i].x;
		next_y += path[i].y;

		if (board[next_x][next_y] != NULL) {
			if (i != path.size() - 1) return false;
			else if (typeid(*board[start.x][start.y]) == typeid(Pawn)) return false;
		}
	}
	return true;
}

bool Player::capturePiece(Position dest) {
	Piece* (*board)[8] = _chessboard->_board;

	if (_playerNum != board[dest.x][dest.y]->getPlayer()) {
		_captureList.push_back(board[dest.x][dest.y]);
		if (typeid(*board[dest.x][dest.y]) == typeid(King)) _winFlag = true;
		return true;
	}
	else return false;
}

bool Player::isCatchKing() {
	return _winFlag;
}

string Player::getCaptureList() {

	string s;

	if (_captureList.size() != 0) {
		s = to_string(_playerNum) + "p: ";

		for (Piece* p : _captureList) {
			s += p -> getCode();
			s += " ";
		}
		s += "\n";
	}
	else {
		s += " ";
	}

	return s;
}

int Player::getPlayerNum() {
	return _playerNum;
}

void Player::reset() {

	list<Piece*>::iterator iter = _captureList.begin();

	for (iter = _captureList.begin(); iter != _captureList.end();) {
		iter = _captureList.erase(iter);
	}
	
	_winFlag = false;
}
