#ifndef __WORLD_H__
#define __WORLD_H__


#include <memory>
#include <vector>

#include "ThreadPool.h"
#include "Tile.h"
#include "Linker.h"
#include "Block.h"
#include "AirSolver.h"
#include "LinkSolver.h"




class World
{
public:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	explicit World(std::size_t size);


private:
	ThreadPool m_threadPool;


protected:
	TileBoard m_board;
	std::vector<std::unique_ptr<Linker>> m_linkers;
	std::vector<std::unique_ptr<Block>> m_blocks;


private:
	AirSolver m_airSolver;
	LinkSolver m_linkSolver;


public:
	void update();
};


#endif