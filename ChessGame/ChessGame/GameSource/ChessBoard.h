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

	void init();	// ���� ���� �ʱ� ����
	string getBoard();	// ���� ���� ���
	int getWidth();
	int getHeight();

private:
	const static int WIDTH = 8;
	const static int HEIGHT = 8;

	Piece* _board[HEIGHT][WIDTH];// ���� ����
	enum { BLACK = 1, WHITE };

	friend class Player;
	
};
