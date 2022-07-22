#include "Piece.h"
class Rook : public Piece
{
public:
	Rook();
	Rook(int player);
	~Rook();
	
	vector<Position> findPath(Position start, Position dest) override;

private:

};