#include "Piece.h"
class Knight : public Piece
{
public:
	Knight();
	Knight(int player);
	~Knight();

	vector<Position> findPath(Position start, Position dest) override;

private:

};