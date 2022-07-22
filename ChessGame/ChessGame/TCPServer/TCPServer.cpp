#define _CRT_SECURE_NO_WARNINGS
#include "TCPServer.h"
#include "GameSource/ChessGame.h"


TCPServer::TCPServer()
{
	_recvMsg = NULL;
}

TCPServer::~TCPServer()
{
}

void TCPServer::start() {

	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
		errorHandling("WSAStartup() error!");

	_hMutex = CreateMutex(NULL, FALSE, NULL);
	if (_hMutex == NULL)
		errorHandling("CreateMutex() error!");

	_acceptSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_acceptSock == INVALID_SOCKET)
		errorHandling("socket() error!");

	memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(PORT);
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_acceptSock, (SOCKADDR*)&_servAddr, sizeof(_servAddr)) == SOCKET_ERROR)
		errorHandling("bind() error!");

	WSAEVENT acceptEvent = WSACreateEvent();
	if (WSAEventSelect(_acceptSock, acceptEvent, FD_ACCEPT) == SOCKET_ERROR)
		errorHandling("WSAEventSelect() error");

	if (listen(_acceptSock, SOMAXCONN) == SOCKET_ERROR)
		errorHandling("listen() error!");

	_clntSocks[_clntNumber] = _acceptSock;
	_hEventArray[_clntNumber++] = acceptEvent;

	int index, i;
	int strLen;
	char message[PACKET_SIZE];

	ChessGame chessgame(this);

	while (TRUE) {
		index = WSAWaitForMultipleEvents(_clntNumber, _hEventArray, FALSE, WSA_INFINITE, FALSE);
		index = index - WSA_WAIT_EVENT_0;

		for (i = index; i < _clntNumber; i++) {
			index = WSAWaitForMultipleEvents(1, &_hEventArray[i], TRUE, 0, FALSE);
			if ((index == WSA_WAIT_FAILED || index == WSA_WAIT_TIMEOUT)) continue;
			else {
				index = i;
				WSAEnumNetworkEvents(_clntSocks[index], _hEventArray[index], &_netEvents);

				if (_netEvents.lNetworkEvents & FD_ACCEPT) {
					if (_netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
						cout << "accept() error!" << endl;
						break;
					}
					_clntAddrSize = sizeof(_clntAddr);
					_clntSock = accept(_clntSocks[index], (SOCKADDR*)&_clntAddr, &_clntAddrSize);
					WSAEVENT hEvent = WSACreateEvent();
					WSAEventSelect(_clntSock, hEvent, FD_READ | FD_CLOSE);

					_hEventArray[_clntNumber] = hEvent;
					_clntSocks[_clntNumber] = _clntSock;

					chessgame.joinUser(_clntNumber++);
					if (_clntNumber == TOTAL_CLIENT) {
						chessgame.init();
						chessgame.start();
					}

					//if (_clntNumber == TOTAL_CLIENT) return true;
				} 
				if (_netEvents.lNetworkEvents & FD_READ) {
					if (_netEvents.iErrorCode[FD_READ_BIT] != 0) {
						cout << "Read Error" << endl;
						break;
					}
					strLen = recv(_clntSocks[index - WSA_WAIT_EVENT_0], message, sizeof(message), 0);
					message[strLen] = '\0';

					int playerIndex = index - WSA_WAIT_EVENT_0;

					cout << playerIndex << "의 메시지 : " << message << endl;
					chessgame.recvMessage(message, playerIndex);

					//return message;

				}// end if (_netEvents.lNetworkEvents & FD_READ)
				 
				if (_netEvents.lNetworkEvents & FD_CLOSE) {
					if (_netEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
						cout << "Close Error" << endl;
					}

					int playerIndex = index - WSA_WAIT_EVENT_0;

					chessgame.exitUser(playerIndex);

					WSACloseEvent(_hEventArray[index]);
					closesocket(_clntSocks[index]);
					cout << "종료된 소켓 핸들 : " << _clntSocks[index] << endl;

					compressSockets(_clntSocks, index, --_clntNumber);
					compressEvents(_hEventArray, index, _clntNumber);
					
				} //end if (_netEvents.lNetworkEvents & FD_CLOSE)

			} //end else

		}//end for
	}

	WSACleanup();

}

void TCPServer::sendMessageTo(const string s, int to) {
	char message[PACKET_SIZE];
	int len = s.length();

	strcpy(message, s.c_str());

	Sleep(100);
	send(_clntSocks[to], message, len, 0);

}

void TCPServer::broadcast(const string s) {

	char message[PACKET_SIZE];
	int len = s.length();

	strcpy(message, s.c_str());

	for (int i = 1; i < TOTAL_CLIENT; i++) {
		Sleep(100);
		send(_clntSocks[i], message, len, 0);
	}
}

void TCPServer::errorHandling(const char* message) {
	cout << message << endl;
	exit(1);
}

void TCPServer::compressSockets(SOCKET* clntSocks, int omitIndex, int total) {
	int i;
	for (i = omitIndex; i < total; i++)
		clntSocks[i] = clntSocks[i + 1];
}

void TCPServer::compressEvents(WSAEVENT* hEventArray, int omitIndex, int total) {
	int i;
	for (i = omitIndex; i < total; i++)
		hEventArray[i] = hEventArray[i + 1];
}