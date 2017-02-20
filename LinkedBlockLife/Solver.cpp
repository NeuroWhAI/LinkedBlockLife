#include "Solver.h"

#include "ThreadPool.h"
#include "Tile.h"




using std::size_t;

//#################################################################################################

Solver::Solver(ThreadPool& threadPool, TileBoard& tileBoard,
	BlockList& blockList, LinkerList& linkerList)
	: m_threadPool(threadPool)
	, m_tileBoard(tileBoard)
	, m_blocks(blockList)
	, m_linkers(linkerList)
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
}

//#################################################################################################

void Solver::foreachTileSafely(std::size_t coreCount)
{
	const size_t boardSize = m_tileBoard.size();

	const size_t heightPerCore = boardSize / coreCount;


	std::vector<std::future<void>> futList;


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&Solver::foreachTileSafelyRange, this,
			core * heightPerCore + 2, heightPerCore - 2);

		futList.emplace_back(std::move(fut));
	}

	foreachTileSafelyRange(2, heightPerCore - 2);


	for (auto& fut : futList)
	{
		fut.wait();
	}


	futList.clear();


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&Solver::foreachTileSafelyRange, this,
			core * heightPerCore, 2);

		futList.emplace_back(std::move(fut));
	}

	foreachTileSafelyRange(0, 2);


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
			core * heightPerCore, heightPerCore);

		futList.emplace_back(std::move(fut));
	}

	foreachTileRange(0, heightPerCore);


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
				(core - 1) * linkerPerCore, linkerPerCore);

			futList.emplace_back(std::move(fut));
		}
	}

	foreachLinkerRange((coreCount - 1) * linkerPerCore, linkerCount - (coreCount - 1) * linkerPerCore);


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
				(core - 1) * blockPerCore, blockPerCore);

			futList.emplace_back(std::move(fut));
		}
	}

	foreachBlockRange((coreCount - 1) * blockPerCore, blockCount - (coreCount - 1) * blockPerCore);


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
			m_moveSolver.updateTile(tile);
		}
	}
}


void Solver::foreachLinkerRange(std::size_t begin, std::size_t count)
{
	const size_t endY = std::min(begin + count, m_linkers.size());


	for (size_t i = begin; i < endY; ++i)
	{
		m_linkSolver.updateLinker(*m_linkers[i]);
	}
}


void Solver::foreachBlockRange(std::size_t begin, std::size_t count)
{
	const size_t boardSize = m_tileBoard.size();
	const size_t endY = std::min(begin + count, m_blocks.size());


	for (size_t b = begin; b < endY; ++b)
	{
		auto& block = *m_blocks[b];

		m_linkSolver.updateBlock(block);
	}
}

