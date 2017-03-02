#ifndef __EXIST_SOLVER_H__
#define __EXIST_SOLVER_H__


#include <memory>
#include <vector>




class Tile;
class Block;


class ExistSolver
{
private:
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;
	using BlockList = std::vector<std::unique_ptr<Block>>;


public:
	explicit ExistSolver(std::size_t coreCount);


private:
	std::vector<std::vector<std::size_t>> m_targetBlocks;
	bool m_needRemove;


public:
	void checkBlock(std::size_t coreIndex, Block& block, std::size_t blockIndex);
	void removeTargetBlocks(BlockList& blocks, TileBoard& board);


private:
	void removeBlockFromWorld(Block& block, TileBoard& board);
};


#endif