#ifndef __EXIST_SOLVER_H__
#define __EXIST_SOLVER_H__


#include <memory>
#include <vector>




class Tile;
class Block;
class Linker;


class ExistSolver
{
private:
	using IndexBoard = std::vector<std::vector<std::size_t>>;
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;
	using BlockList = std::vector<std::unique_ptr<Block>>;
	using LinkerList = std::vector<std::unique_ptr<Linker>>;


public:
	explicit ExistSolver(std::size_t coreCount);


private:
	std::size_t m_currentCore;
	std::vector<std::size_t>::const_iterator m_currentItr;
	std::vector<std::size_t>::const_iterator m_currentEnd;


private:
	IndexBoard m_targetBlocks;
	bool m_needRemoveBlock;

	IndexBoard m_targetLinkers;
	bool m_needRemoveLinker;


private:
	void updateIterator(IndexBoard& indexBoard);
	template <typename ListType, typename WD, typename... WD_ARGS>
	void removeTarget(ListType& list, IndexBoard& indexBoard,
		WD&& removeFromWorld, WD_ARGS&&... args);


public:
	void checkBlock(std::size_t coreIndex, Block& block, std::size_t blockIndex);
	void removeTargetBlocks(BlockList& blocks, TileBoard& board);

	void checkLinker(std::size_t coreIndex, Linker& linker, std::size_t linkerIndex);
	void removeTargetLinkers(LinkerList& linkers);


private:
	void removeBlockFromWorld(Block& block, TileBoard& board);
	void removeLinkerFromWorld(Linker& linker);
};


#include "ExistSolver.inl"


#endif