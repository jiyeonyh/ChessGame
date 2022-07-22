#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop();
	Bishop(int player);
	~Bishop();

	vector<Position> findPath(Position start, Position dest) override;

private:
	
};