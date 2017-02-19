#ifndef __SOLVER_H__
#define __SOLVER_H__


#include <memory>
#include <vector>

#include "AirSolver.h"
#include "LinkSolver.h"




class ThreadPool;
class Tile;
class Block;
class Linker;


class Solver
{
public:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;
	using BlockList = std::vector<std::unique_ptr<Block>>;
	using LinkerList = std::vector<std::unique_ptr<Linker>>;


public:
	Solver(ThreadPool& threadPool, TileBoard& tileBoard,
		BlockList& blockList, LinkerList& linkerList);


private:
	ThreadPool& m_threadPool;
	TileBoard& m_tileBoard;
	BlockList& m_blocks;
	LinkerList& m_linkers;


private:
	AirSolver m_airSolver;
	LinkSolver m_linkSolver;


public:
	void solve();


private:
	void foreachTileSafely(std::size_t coreCount);
	void foreachTile(std::size_t coreCount);
	void foreachLinker(std::size_t coreCount);
	void foreachBlock(std::size_t coreCount);


private:
	void foreachTileSafelyRange(std::size_t beginY, std::size_t count);
	void foreachTileRange(std::size_t beginY, std::size_t count);
	void foreachLinkerRange(std::size_t begin, std::size_t count);
	void foreachBlockRange(std::size_t begin, std::size_t count);
};


#endif