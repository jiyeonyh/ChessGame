#include "Piece.h"

class King : public Piece
{
public:
	King();
	King(int player);
	~King();

	vector<Position> findPath(Position start, Position dest) override;

private:

};