#define _CRT_SECURE_NO_WARNINGS
#include "ChessGame.h"

#define PACKET_SIZE 1024
#define TOTAL_CLIENT 3

ChessGame::ChessGame(TCPServer* server)
{
	_server = server;
	_p1 = new Player(BLACK, &_chessboard);
	_p2 = new Player(WHITE, &_chessboard);

}

ChessGame::~ChessGame()
{
	delete _p1;
	delete _p2;
}

void ChessGame::init()
{
	_chessboard.init();
}

void ChessGame::start()
{
	_server->broadcast(to_string(START) + "|");
	_currentPlayer = _p1;
	_readyCount = 0;
	_state = GameState::CHECK_READY;
}

Player* ChessGame::getPlayerBy(int index)
{
	if (_p1->getPlayerNum() == index) {
		return _p1;
	}

	return _p2;
}

Player* ChessGame::getNextPlayer() {
	if (_currentPlayer == _p1) {
		return _p2;
	}

	return _p1;
}

void ChessGame::recvMessage(const std::string& msg, int index)
{
	//const int playerIndex = index - 1;
	Player* curPlayer = getPlayerBy(index);
	Player* nextPlayer = getNextPlayer();

	if (_state == GameState::CHECK_READY) {
		if (msg[0] == 'N' || msg[0] == 'n') {
			exitUser(index);
		}
		else {
			_readyCount++;
			if (_readyCount == 2) {
				_state = GameState::SELECT_PIECE;
				_server->sendMessageTo(to_string(SELECT_START) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
				_server->sendMessageTo(to_string(WAIT) + "|" + getChessBoardInfo(), getNextPlayer()->getPlayerNum());
			}
		}
	}
	else {

		if (curPlayer != _currentPlayer) {
			return;
		}

		switch (_state) {
			case GameState::SELECT_PIECE: {
				bool isCorrect = this->isCorrect(msg);
				if (isCorrect) {
					_startPos.setPosition(stringToPosition(msg));
					isCorrect = _currentPlayer->selectPiece(_startPos);
					if (isCorrect) {
						_server->sendMessageTo(to_string(SELECT_DEST) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
						_state = GameState::MOVE_PIECE;
					}
				}
				if (!isCorrect) {
					_server->sendMessageTo(to_string(RETRY) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
				}
				break;
			}
			case GameState::MOVE_PIECE: {
				if (msg.compare("/back") == 0) {
					_server->sendMessageTo(to_string(SELECT_START) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
					_state = GameState::SELECT_PIECE;
				}
				else {
					if (isCorrect(msg)) {
						_destPos.setPosition(stringToPosition(msg));

						if (_currentPlayer->movePiece(_startPos, _destPos)) {
							_server->sendMessageTo(to_string(MOVE) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
							_server->sendMessageTo(to_string(OPNT_MOVE) + "|" + getChessBoardInfo(), getNextPlayer()->getPlayerNum());
							Sleep(300);

							if (_currentPlayer->isCatchKing()) {
								_server->sendMessageTo(to_string(WIN) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
								_server->sendMessageTo(to_string(LOSE) + "|" + getChessBoardInfo(), getNextPlayer()->getPlayerNum());

								_p1->reset();
								_p2->reset();

								init();
								start();
							}
							else {
								changePlayer(&_currentPlayer, &nextPlayer);

								_state = GameState::SELECT_PIECE;
								_server->sendMessageTo(to_string(SELECT_START) + "|" + getChessBoardInfo(), _currentPlayer->getPlayerNum());
								_server->sendMessageTo(to_string(WAIT) + "|" + getChessBoardInfo(), getNextPlayer()->getPlayerNum());
							}
							break;
						}

					} //end if(isCorrect(msg))
					_server->sendMessageTo(to_string(CANT_MOVE) + "|" + getChessBoardInfo(), curPlayer->getPlayerNum());
				}//end else 
				break;
			}
		}
	}
}

void ChessGame::joinUser(int index)
{
	_userCount++;

	char acceptMsg[PACKET_SIZE];
	acceptMsg[0] = '0' + ACCEPT;
	acceptMsg[1] = '|';
	acceptMsg[2] = NULL;

	int length = strlen(acceptMsg);
	_server->sendMessageTo(acceptMsg, index);
}

void ChessGame::exitUser(int index)
{
	_server->sendMessageTo(to_string(QUIT) + "|", index);

	if (_p1->getPlayerNum() == index)
		_server->sendMessageTo(to_string(OPNT_QUIT) + "|", _p2->getPlayerNum());
	else
		_server->sendMessageTo(to_string(OPNT_QUIT) + "|", _p1->getPlayerNum());
	
	_userCount--;
}


/*입력값 유효성 체크*/
bool ChessGame::isCorrect(string s) {
	int length = s.size();
	if (length != 2) return false;

	int x;
	int y = s.at(1) - '1';

	if (s.at(0) >= 'a') x = s.at(0) - 'a';
	else x = s.at(0) - 'A';

	if (x < 0 || x >= _chessboard.getHeight() || y < 0 || y >= _chessboard.getWidth()) return false;

	return true;
}

Position ChessGame::stringToPosition(string s) {
	Position p;

	if (s.at(0) >= 'a') p.setPosition(s.at(0) - 'a', s.at(1) - '1');
	else p.setPosition(s.at(0) - 'A', s.at(1) - '1');;

	return p;
}

void ChessGame::changePlayer(Player** curPlayer, Player** nextPlayer)
{
	if ((*curPlayer)->getPlayerNum() == BLACK) {
		*curPlayer = _p2;
		*nextPlayer = _p1;
	}
	else {
		*curPlayer = _p1;
		*nextPlayer = _p2;
	}
}

string ChessGame::getChessBoardInfo() {

	string boardString = _chessboard.getBoard();
	
	boardString += "|";
	boardString += _p1->getCaptureList();

	boardString += "|";
	boardString += _p2->getCaptureList();

	return boardString;

}