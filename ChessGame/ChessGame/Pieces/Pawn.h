#include "Piece.h"
class Pawn : public Piece
{
public:
	Pawn();
	Pawn(int player);
	~Pawn();

	vector<Position> findPath(Position start, Position dest) override;
	bool attack(Position start, Position dest);
	void setMoveState(bool state);

private:
	bool _moveState;
};	
