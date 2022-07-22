#include "Piece.h"
class Queen : public Piece
{
public:
	Queen();
	Queen(int player);
	~Queen();

	vector<Position> findPath(Position start, Position dest) override;

private:
};