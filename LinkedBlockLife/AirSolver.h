#ifndef __AIR_SOLVER_H__
#define __AIR_SOLVER_H__


#include <memory>
#include <vector>




class ThreadPool;
class Tile;


class AirSolver
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	AirSolver(TileBoard& board, ThreadPool& threadPool);


private:
	TileBoard& m_board;
	ThreadPool& m_threadPool;


public:
	void solve();


private:
	void updateRange(std::size_t beginY, std::size_t count);
	void updateTileRange(std::size_t beginY, std::size_t count);
};


#endif