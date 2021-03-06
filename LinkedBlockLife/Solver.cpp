#include "Solver.h"

#include <cassert>

#include "ThreadPool.h"
#include "Tile.h"
#include "Processor.h"




using std::size_t;

//#################################################################################################

Solver::Solver(ThreadPool& threadPool,
	WorldInteractor& interactor, TileBoard& tileBoard,
	BlockList& blockList, LinkerList& linkerList,
	ProcList& procList)
	: m_threadPool(threadPool)
	, m_interactor(interactor)
	, m_tileBoard(tileBoard)
	, m_blocks(blockList)
	, m_linkers(linkerList)
	, m_procs(procList)

	, m_existSolver(std::thread::hardware_concurrency())
	, m_jobSolver(std::thread::hardware_concurrency())
{

}

//#################################################################################################

void Solver::solve()
{
	const auto coreCount = std::thread::hardware_concurrency();

	foreachTileSafely(coreCount);
	foreachTile(coreCount);
	foreachLinker(coreCount);
	foreachBlock(coreCount);
	foreachProc(coreCount);

	m_jobSolver.performAllJobs(m_interactor);

	m_existSolver.removeTargetBlocks(m_blocks, m_tileBoard);
	m_existSolver.removeTargetLinkers(m_linkers);
	m_existSolver.removeTargetProcessors(m_procs);
}

//#################################################################################################

void Solver::foreachTileSafely(std::size_t coreCount)
{
	const size_t boardSize = m_tileBoard.size();

	const size_t heightPerCore = boardSize / coreCount;


	assert(heightPerCore > 2);


	std::vector<std::future<void>> futList;


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&Solver::foreachTileSafelyRange, this,
			(core - 1) * heightPerCore + 2, heightPerCore - 2);

		futList.emplace_back(std::move(fut));
	}

	foreachTileSafelyRange((coreCount - 1) * heightPerCore + 2,
		boardSize - ((coreCount - 1) * heightPerCore + 2));


	for (auto& fut : futList)
	{
		fut.wait();
	}


	futList.clear();


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&Solver::foreachTileSafelyRange, this,
			(core - 1) * heightPerCore, 2);

		futList.emplace_back(std::move(fut));
	}

	foreachTileSafelyRange((coreCount - 1) * heightPerCore, 2);


	for (auto& fut : futList)
	{
		fut.wait();
	}
}


void Solver::foreachTile(std::size_t coreCount)
{
	const size_t boardSize = m_tileBoard.size();

	const size_t heightPerCore = boardSize / coreCount;


	std::vector<std::future<void>> futList;


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&Solver::foreachTileRange, this,
			(core - 1) * heightPerCore, heightPerCore);

		futList.emplace_back(std::move(fut));
	}

	foreachTileRange((coreCount - 1) * heightPerCore,
		boardSize - (coreCount - 1) * heightPerCore);


	for (auto& fut : futList)
	{
		fut.wait();
	}
}


void Solver::foreachLinker(std::size_t coreCount)
{
	const size_t linkerCount = m_linkers.size();

	const size_t linkerPerCore = linkerCount / coreCount;


	std::vector<std::future<void>> futList;


	if (linkerPerCore > 0)
	{
		for (size_t core = 1; core < coreCount; ++core)
		{
			auto fut = m_threadPool.reserve(&Solver::foreachLinkerRange, this,
				core, (core - 1) * linkerPerCore, linkerPerCore);

			futList.emplace_back(std::move(fut));
		}
	}

	foreachLinkerRange(0, (coreCount - 1) * linkerPerCore,
		linkerCount - (coreCount - 1) * linkerPerCore);


	for (auto& fut : futList)
	{
		fut.wait();
	}
}


void Solver::foreachBlock(std::size_t coreCount)
{
	const size_t blockCount = m_blocks.size();

	const size_t blockPerCore = blockCount / coreCount;


	std::vector<std::future<void>> futList;


	if (blockPerCore > 0)
	{
		for (size_t core = 1; core < coreCount; ++core)
		{
			auto fut = m_threadPool.reserve(&Solver::foreachBlockRange, this,
				core, (core - 1) * blockPerCore, blockPerCore);

			futList.emplace_back(std::move(fut));
		}
	}

	foreachBlockRange(0, (coreCount - 1) * blockPerCore,
		blockCount - (coreCount - 1) * blockPerCore);


	for (auto& fut : futList)
	{
		fut.wait();
	}
}


void Solver::foreachProc(std::size_t coreCount)
{
	const size_t procCount = m_procs.size();

	const size_t procPerCore = procCount / coreCount;


	std::vector<std::future<void>> futList;


	if (procPerCore > 0)
	{
		for (size_t core = 1; core < coreCount; ++core)
		{
			auto fut = m_threadPool.reserve(&Solver::foreachProcRange, this,
				core, (core - 1) * procPerCore, procPerCore);

			futList.emplace_back(std::move(fut));
		}
	}

	foreachProcRange(0, (coreCount - 1) * procPerCore,
		procCount - (coreCount - 1) * procPerCore);


	for (auto& fut : futList)
	{
		fut.wait();
	}
}

//#################################################################################################

void Solver::foreachTileSafelyRange(std::size_t beginY, std::size_t count)
{
	const size_t boardSize = m_tileBoard.size();
	const size_t endY = std::min(beginY + count, boardSize);


	for (size_t y = beginY; y < endY; ++y)
	{
		auto& line = m_tileBoard[y];

		for (size_t x = 0; x < boardSize; ++x)
		{
			auto& tile = *line[x];

			auto& blocks = tile.getBlocks();
			if (!blocks.empty())
			{
				auto blockDensity = blocks.size();

				for (auto* pBlock : blocks)
				{
					m_moveSolver.updateBlock(m_tileBoard, boardSize, *pBlock, tile, blockDensity);
				}
			}

			m_airSolver.updateNearTile(m_tileBoard, tile, x, y);
		}
	}
}


void Solver::foreachTileRange(std::size_t beginY, std::size_t count)
{
	const size_t boardSize = m_tileBoard.size();
	const size_t endY = std::min(beginY + count, m_tileBoard.size());


	for (size_t y = beginY; y < endY; ++y)
	{
		auto& line = m_tileBoard[y];

		for (size_t x = 0; x < boardSize; ++x)
		{
			auto& tile = *line[x];

			m_airSolver.updateTile(tile, boardSize, x, y);
			m_moveSolver.updateTile(tile, x, y);
		}
	}
}


void Solver::foreachLinkerRange(std::size_t coreIndex, std::size_t begin, std::size_t count)
{
	const size_t endY = std::min(begin + count, m_linkers.size());


	for (size_t i = begin; i < endY; ++i)
	{
		auto& linker = *m_linkers[i];

		m_linkSolver.updateLinker(linker);
		m_existSolver.checkLinker(coreIndex, linker, i);
	}
}


void Solver::foreachBlockRange(std::size_t coreIndex, std::size_t begin, std::size_t count)
{
	const size_t endY = std::min(begin + count, m_blocks.size());


	for (size_t b = begin; b < endY; ++b)
	{
		auto& block = *m_blocks[b];

		m_linkSolver.updateBlock(block);
		m_existSolver.checkBlock(coreIndex, block, b);
	}
}


void Solver::foreachProcRange(std::size_t coreIndex, std::size_t begin, std::size_t count)
{
	const size_t endY = std::min(begin + count, m_procs.size());


	for (size_t p = begin; p < endY; ++p)
	{
		auto& proc = *m_procs[p];

		m_jobSolver.updateProcessor(coreIndex, proc, m_interactor);
		m_existSolver.checkProcessor(coreIndex, proc, p);
	}
}

