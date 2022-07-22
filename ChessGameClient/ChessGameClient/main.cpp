#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <vector>
#include <sstream>

#pragma comment(lib,"ws2_32")

#define PORT 4578
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.1.247"

using namespace std;

DWORD WINAPI sendMessage(const char* message);
DWORD WINAPI recvMessage(void* arg);
vector<string> split(string sentence, char Separator);
void setPlayerNum(int num);
void print(vector<string>& recv);
void doCommend(int commend, vector<string>& recv);
void errorHandling(const char* message);

char message[PACKET_SIZE];
enum {
	ACCEPT, START, SELECT_START, SELECT_DEST, MOVE,
	CANT_MOVE, OPNT_MOVE, WAIT, RETRY, SUCCESS, WIN, LOSE, QUIT, OPNT_QUIT
};
SOCKET sock;
string startInput;
string destInput;
bool playFlag;
int playerNum;

int main() {
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	HANDLE hThread;
	DWORD dwThreadID;

	playFlag = false;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		errorHandling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		errorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		errorHandling("connect() error!");

	hThread = (HANDLE)CreateThread(NULL, 0, recvMessage, (void*)sock, 0, &dwThreadID);

	if (hThread == 0)
		errorHandling("������ ���� ����");

	WaitForSingleObject(hThread, INFINITE);

	closesocket(sock);

	return 0;
}

DWORD WINAPI sendMessage(const char* message) {

	return send(sock, message, strlen(message), 0);
}

DWORD WINAPI recvMessage(void* arg) {
	SOCKET sock = (SOCKET)arg;
	char recvMsg[PACKET_SIZE];
	int strLen;

	while (TRUE) {
		strLen = recv(sock, recvMsg, PACKET_SIZE, 0);
		if (strLen == -1) return 1;

		recvMsg[strLen] = 0;

		string recvString(recvMsg);
		vector<string> recv = split(recvString, '|');

		int command = stoi(recv[0]);
		doCommend(command, recv);
	}

}

void doCommend(int commend, vector<string>& recv) {
	string sel;
	switch (commend) {
	case ACCEPT:
		cout << "ü�� ���� ���� ���� �Ϸ�. ������ ��ٸ��� ���Դϴ�...\n";
		break;

	case START:
		cout << " ü�� ������"; if (playFlag) cout << " �ٽ�";
		cout << " �����Ͻðڽ��ϱ� ? (Y / N) "; cin >> sel;
		sendMessage(sel.c_str());
		break;

	case SELECT_START:
		playFlag = true;
		print(recv);
		cout << "�÷��̾�� �����Դϴ�. �ű� �⹰�� �������ּ���. ex) A1" << endl;
		cout << " �Է� : "; cin >> startInput;
		sendMessage(startInput.c_str());
		break;

	case SELECT_DEST:
		print(recv);
		cout << "������ �Ϸ�Ǿ����ϴ�!\n";
		cout << " �⹰�� ���� �ű�ðڽ��ϱ�? ������ ��ǥ : "
			<< (char)toupper(startInput.at(0)) << startInput.at(1) << "( ���ư��� '/back' )" << endl;
		cout << " �Է� : "; cin >> destInput;
		sendMessage(destInput.c_str());
		break;

	case MOVE:
		print(recv);
		cout << "�̵��� �Ϸ�Ǿ����ϴ�!" << endl;
		break;

	case CANT_MOVE:
		print(recv);
		cout << "�̵��� �� ���� ��ġ�Դϴ�.�ٽ� �Է����ּ���!" << endl;
		cout << " �⹰�� ���� �ű�ðڽ��ϱ�? ������ ��ǥ : "
			<< (char)toupper(startInput.at(0)) << startInput.at(1) << "( ���ư��� '/back' )" << endl;
		cout << " �Է� : "; cin >> destInput;
		sendMessage(destInput.c_str());
		break;

	case OPNT_MOVE:
		print(recv);
		cout << "������ �⹰�� �̵����׽��ϴ�!" << endl;
		break;

	case WAIT:
		print(recv);
		cout << "���� �����Դϴ�. ��� �÷��̾� �Է���...\n";
		break;

	case RETRY:
		print(recv);
		cout << "�߸��Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���!" << endl;
		cout << " �Է� : "; cin >> startInput;
		sendMessage(startInput.c_str());
		break;

	case SUCCESS:
		cout << "������ �Ϸ�Ǿ����ϴ�!\n";
		break;

	case WIN:
		cout << " WIN!!! ŷ�� ��ҽ��ϴ�! �÷��̾���� �¸��ϼ̽��ϴ�~!" << endl;
		break;

	case LOSE:
		cout << " LOSE!! ŷ�� �������ϴ�. �÷��̾���� ���ϼ̽��ϴ�..!" << endl;
		break;

	case QUIT:
		cout << "������ �����ϰڽ��ϴ�..." << endl;
		exit(0);
		break;

	case OPNT_QUIT:
		cout << "������ ������ �����߽��ϴ�. ������ �����մϴ�..." << endl;
		exit(0);
		break;

	default:
		break;
	}
}

void setPlayerNum(int num) {
	playerNum = num;
}

void print(vector<string>& recv) {
	vector<string> board = split(recv[1], ',');
	system("cls");
	char c[8] = { 'A','B','C','D','E','F','G','H' };

	cout << "      1   2   3   4   5   6   7   8" << endl;
	cout << "    --------------------------------" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << " " << c[i] << " | ";
		for (int j = 0; j < 8; j++) {
			cout << board[i * 8 + j] << " | ";
		}
		cout << endl << "    --------------------------------" << endl;
	}

	cout << recv[2] << endl;
	cout << recv[3] << "\n\n\n ";
}
vector<string> split(string sentence, char Separator)
{
	vector<string> answer;
	stringstream ss(sentence);
	string tmp;

	while (getline(ss, tmp, Separator)) {
		answer.push_back(tmp);
	}
	return answer;
}

void errorHandling(const char* message) {
	cout << message << endl;
	exit(1);
}