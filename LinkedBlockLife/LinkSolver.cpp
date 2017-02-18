#include "LinkSolver.h"

#include <thread>
#include <future>

#include "ThreadPool.h"
#include "Block.h"
#include "Linker.h"




using std::size_t;

//#################################################################################################

LinkSolver::LinkSolver(BlockList& blockList, ThreadPool& threadPool)
	: m_blocks(blockList)
	, m_threadPool(threadPool)
{

}

//#################################################################################################

void LinkSolver::solve()
{
	const size_t blockCount = m_blocks.size();
	const size_t coreCount = std::thread::hardware_concurrency();
	const size_t countPerCore = blockCount / coreCount;


	std::vector<std::future<void>> futList;
	futList.reserve(coreCount - 1);


	if (countPerCore > 0)
	{
		for (size_t core = 1; core < coreCount; ++core)
		{
			auto fut = m_threadPool.reserve(&LinkSolver::updateRange, this,
				(core - 1) * countPerCore, countPerCore);

			futList.emplace_back(std::move(fut));
		}
	}

	updateRange((coreCount - 1) * countPerCore, blockCount - (coreCount - 1) * countPerCore);

	for (auto& fut : futList)
	{
		fut.wait();
	}
}

//#################################################################################################

void LinkSolver::updateRange(std::size_t beginY, std::size_t count)
{
	const size_t endY = std::min(beginY + count, m_blocks.size());


	for (size_t b = beginY; b < endY; ++b)
	{
		auto& block = m_blocks[b];
		auto& linkerPort = block->getLinkerPort();

		auto& linkers = linkerPort.getLinkerList();

		for (auto* linker : linkers)
		{
			auto elasticity = linker->calculateElasticity();

			if (&linker->getFirst() == block.get())
			{
				elasticity *= -1.0f;
			}

			block->addSpeed(elasticity);
		}
	}
}

