#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "GameSource/ChessGame.h"
#include "TCPServer/TCPServer.h"
#include <process.h>

int main() {
	TCPServer server;

	server.start();
	
	
	return 0;
}//end main
