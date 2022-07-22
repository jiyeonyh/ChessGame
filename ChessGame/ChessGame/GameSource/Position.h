#pragma once
struct Position {
	int x;
	int y;

	Position() {
		x = 0;
		y = 0;
	}
	Position(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void setPosition(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void setPosition(const Position& p) {
		x = p.x;
		y = p.y;
	}

	Position operator+(const Position& p) {
		return Position(this->x + p.x, this->y + p.y);
	}
	Position operator-(const Position& p) {
		return Position(this->x - p.x, this->y - p.y);
	}
};