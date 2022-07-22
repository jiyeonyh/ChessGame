#pragma once
#include "ChessBoard.h"
#include "Player.h"
#include <WinSock2.h>
#include <TCPServer/TCPServer.h>
#include "GameState.h"

class ChessGame
{
public:
	ChessGame(TCPServer* server);
	~ChessGame();

	void init();
	void start();
	bool isCorrect(string s);
	Position stringToPosition(string s);
	void changePlayer(Player** curPlayer, Player** nextPlayer);
	string getChessBoardInfo();
	void recvMessage(const std::string& message, int index);
	void joinUser(int index);
	void exitUser(int index);

private:
	Player* getPlayerBy(int index);
	Player* getNextPlayer();

private:
	enum { BLACK = 1, WHITE = 2 };
	enum { ACCEPT, START, SELECT_START, SELECT_DEST, MOVE, 
		CANT_MOVE, OPNT_MOVE, WAIT, RETRY, SUCCESS, WIN, LOSE, QUIT, OPNT_QUIT
	};

	TCPServer* _server;
	ChessBoard _chessboard;
	GameState _state = GameState::NONE;

	Player* _p1;
	Player* _p2;
	Player* _currentPlayer;

	
	Position _startPos;
	Position _destPos;

	int _userCount = 0;
	int _readyCount = 0;
};
