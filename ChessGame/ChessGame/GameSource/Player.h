#pragma once
#include "ChessBoard.h"
#include "Pieces/Piece.h"
#include <list>

class Player
{
public:
	Player();
	Player(int playerNum, ChessBoard* chessboard);
	~Player();

	bool selectPiece(Position start);
	bool movePiece(Position start, Position dest);
	bool canMove(Position start, const vector<Position>& path);
	bool capturePiece(Position dest);
	bool isCatchKing();
	string getCaptureList();
	int getPlayerNum();
	void reset();
	

private:
	ChessBoard* _chessboard;
	list<Piece*> _captureList;
	enum { BLACK = 1, WHITE };
	enum { UPPERCASE = 1, LOWERCASE };
	int _playerNum;
	bool _winFlag;
};