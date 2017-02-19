#ifndef __AIR_SOLVER_H__
#define __AIR_SOLVER_H__


#include <memory>
#include <vector>




class Tile;


class AirSolver
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	AirSolver();


public:
	void updateNearTile(TileBoard& board, Tile& here, std::size_t x, std::size_t y);
	void updateTile(Tile& tile);
};


#endif