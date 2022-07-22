#pragma once
#include <iostream>
#include <vector>
#include "GameSource/Position.h"

using namespace std;

class Piece
{
public:

	Piece();
	Piece(int player);
	virtual ~Piece();

	virtual vector<Position> findPath(Position start, Position dest) = 0;
	int getPlayer();
	void setCode(char _code);
	char getCode();

private:
	int _player;
	char _code;
};
