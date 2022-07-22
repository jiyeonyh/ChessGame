#pragma once
#include <iostream>
#include <string>
#include <typeinfo>
#include "Pieces/Piece.h"

using namespace std;
class ChessBoard
{
public:
	ChessBoard();
	~ChessBoard();

	void init();	// 게임 보드 초기 설정
	string getBoard();	// 현재 보드 출력
	int getWidth();
	int getHeight();

private:
	const static int WIDTH = 8;
	const static int HEIGHT = 8;

	Piece* _board[HEIGHT][WIDTH];// 게임 보드
	enum { BLACK = 1, WHITE };

	friend class Player;
	
};
