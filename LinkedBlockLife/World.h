#ifndef __WORLD_H__
#define __WORLD_H__


#include <memory>
#include <vector>

#include "ThreadPool.h"
#include "WorldInteractor.h"
#include "Solver.h"
#include "Tile.h"
#include "Linker.h"
#include "Block.h"
#include "Processor.h"




class World
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	explicit World(std::size_t size);
	virtual ~World() = default;


private:
	ThreadPool m_threadPool;


private:
	Solver m_solver;
	WorldInteractor m_interactor;


protected:
	TileBoard m_board;
	std::vector<std::unique_ptr<Linker>> m_linkers;
	std::vector<std::unique_ptr<Block>> m_blocks;
	std::vector<std::unique_ptr<Processor>> m_processors;


public:
	std::size_t getBoardSize() const;


public:
	void update();


protected:
	Block* addBlock(std::size_t x, std::size_t y, int data = 0);
	Linker* addLinker(Block& first, Block& second);
	Processor* addProcessor(Block* pBlock, const caDraw::VectorF& dir);
};


#endif