#pragma once
#include <WinSock2.h>
#include <process.h>
#include <string>

#pragma comment(lib,"ws2_32")	

#define PORT 4578
#define PACKET_SIZE 1024
#define TOTAL_CLIENT 3

using namespace std;

class TCPServer
{
public:
	TCPServer();
	~TCPServer();
	
	void start();
	void sendMessageTo(const string s, int to);
	void broadcast(const string s);
	void errorHandling(const char* message);
	void compressSockets(SOCKET* clntSocks, int omitIndex, int total);
	void compressEvents(WSAEVENT* hEventArray, int omitIndex, int total);

private:
	enum {
		ACCEPT, START, SELECT_START, SELECT_DEST, MOVE,
		CANT_MOVE, OPNT_MOVE, WAIT, RETRY, SUCCESS, WIN, LOSE, QUIT
	};
	int _clntNumber = 0;
	SOCKET _clntSocks[TOTAL_CLIENT];
	HANDLE _hMutex;

	WSADATA _wsaData;
	SOCKET _acceptSock;
	SOCKET _clntSock;

	SOCKADDR_IN _servAddr;
	SOCKADDR_IN _clntAddr;
	int _clntAddrSize;
	char* _recvMsg;

	WSAEVENT _hEventArray[WSA_MAXIMUM_WAIT_EVENTS];
	WSANETWORKEVENTS _netEvents;

};