#ifndef __WORLD_INTERACTOR_H__
#define __WORLD_INTERACTOR_H__


#include <memory>
#include <vector>

#include "CodeAdapter\EasyCA.h"




class Tile;
class Block;
class Linker;
class Processor;
class LinkerPort;


class WorldInteractor
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;
	using BlockList = std::vector<std::unique_ptr<Block>>;
	using LinkerList = std::vector<std::unique_ptr<Linker>>;
	using ProcList = std::vector<std::unique_ptr<Processor>>;


public:
	WorldInteractor(TileBoard& tileBoard, BlockList& blockList,
		LinkerList& linkerList, ProcList& procList);


private:
	TileBoard& m_tileBoard;
	BlockList& m_blocks;
	LinkerList& m_linkers;
	ProcList& m_procs;


public:
	Block* addBlock(std::size_t x, std::size_t y, int data = 0);
	Linker* addLinker(Block& first, Block& second);
	Processor* addProcessor(Block* pBlock, const caDraw::VectorF& dir);


public:
	void connectLinkerToNear(Block* pCenter);


private:
	void tryConnectLinkerTo(std::size_t x, std::size_t y, Block* pCenter, LinkerPort& port);
};


#endif