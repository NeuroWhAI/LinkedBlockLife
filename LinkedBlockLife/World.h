#ifndef __WORLD_H__
#define __WORLD_H__


#include <memory>
#include <vector>

#include "ThreadPool.h"
#include "Tile.h"
#include "AirSolver.h"




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


private:
	AirSolver m_airSolver;


public:
	void update();
};


#endif