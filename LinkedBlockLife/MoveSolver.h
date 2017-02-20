#ifndef __MOVE_SOLVER_H__
#define __MOVE_SOLVER_H__


#include <memory>
#include <vector>




class Tile;
class Block;


class MoveSolver
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	MoveSolver();


public:
	void updateTile(Tile& tile);
	void updateBlock(TileBoard& board, std::size_t boardSize, Block& block, Tile& tile,
		std::size_t density);
};


#endif