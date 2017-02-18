#ifndef __LINK_SOLVER_H__
#define __LINK_SOLVER_H__


#include <memory>
#include <vector>




class ThreadPool;
class Block;


class LinkSolver
{
private:
	using BlockList = std::vector<std::unique_ptr<Block>>;


public:
	LinkSolver(BlockList& blockList, ThreadPool& threadPool);


private:
	BlockList& m_blocks;
	ThreadPool& m_threadPool;


public:
	void solve();


private:
	void updateRange(std::size_t beginY, std::size_t count);
};


#endif