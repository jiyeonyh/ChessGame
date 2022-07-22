#define _CRT_SECURE_NO_WARNINGS
#include "ChessBoard.h"
#include "Pieces/Piece.h"
#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"
#include <windows.h>

#define PACKET_SIZE 1024

ChessBoard::ChessBoard()
{
	init();
}
ChessBoard::~ChessBoard()
{
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			delete _board[i][j];

}
/*게임 보드 초기 설정*/
void ChessBoard::init() {
	memset(_board, 0, sizeof(Piece*) * WIDTH * HEIGHT);

	_board[0][0] = new Rook(BLACK);		_board[0][1] = new Knight(BLACK);	_board[0][2] = new Bishop(BLACK);	_board[0][3] = new King(BLACK);
	_board[0][4] = new Queen(BLACK);	_board[0][5] = new Bishop(BLACK);	_board[0][6] = new Knight(BLACK);	_board[0][7] = new Rook(BLACK);

	for (int i = 0; i < 8; i++) _board[1][i] = new Pawn(BLACK);
	for (int i = 0; i < 8; i++) _board[6][i] = new Pawn(WHITE);
	
	_board[7][0] = new Rook(WHITE);		_board[7][1] = new Knight(WHITE);	_board[7][2] = new Bishop(WHITE);	_board[7][3] = new King(WHITE);
	_board[7][4] = new Queen(WHITE);	_board[7][5] = new Bishop(WHITE);	_board[7][6] = new Knight(WHITE);	_board[7][7] = new Rook(WHITE);
}

/*현재 보드 출력*/
string ChessBoard::getBoard() {
	string s = "";

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++) {
			if (_board[i][j] != NULL) {
				s += _board[i][j]->getCode();
				s += ",";
			}
			else {
				s += "*";
				s += ",";
			}
		}
	}
	return s;
}

int ChessBoard::getWidth() {
	return WIDTH;
}
int ChessBoard::getHeight() {
	return HEIGHT;
}
