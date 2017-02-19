#ifndef __WORLD_H__
#define __WORLD_H__


#include <memory>
#include <vector>

#include "ThreadPool.h"
#include "Solver.h"
#include "Tile.h"
#include "Linker.h"
#include "Block.h"




class World
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	explicit World(std::size_t size);


private:
	ThreadPool m_threadPool;


private:
	Solver m_solver;


protected:
	TileBoard m_board;
	std::vector<std::unique_ptr<Linker>> m_linkers;
	std::vector<std::unique_ptr<Block>> m_blocks;


public:
	void update();
};


#endif