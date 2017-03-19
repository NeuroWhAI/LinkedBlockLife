#include "WorldInteractor.h"

#include "Tile.h"
#include "Block.h"
#include "Linker.h"
#include "Processor.h"




WorldInteractor::WorldInteractor(TileBoard& tileBoard, BlockList& blockList,
	LinkerList& linkerList, ProcList& procList)
	: m_tileBoard(tileBoard)
	, m_blocks(blockList)
	, m_linkers(linkerList)
	, m_procs(procList)
{

}

//#################################################################################################

Block* WorldInteractor::addBlock(std::size_t x, std::size_t y, int data)
{
	return addBlock({ static_cast<float>(x), static_cast<float>(y) }, data);
}


Block* WorldInteractor::addBlock(const caDraw::VectorF& position, int data)
{
	auto block = std::make_unique<Block>();

	Block* ptr = block.get();


	const caDraw::VectorT<size_t> tileCoord{
		static_cast<size_t>(position.x + 0.5f),
		static_cast<size_t>(position.y + 0.5f)
	};


	block->setPosition(position);
	block->setData(data);

	m_tileBoard[tileCoord.y][tileCoord.x]->addBlock(ptr);

	m_blocks.emplace_back(std::move(block));


	return ptr;
}


Linker* WorldInteractor::addLinker(Block& first, Block& second)
{
	auto linker = std::make_unique<Linker>(first, second);

	Linker* ptr = linker.get();


	first.getLinkerPort().connect(ptr);
	second.getLinkerPort().connect(ptr);


	m_linkers.emplace_back(std::move(linker));


	return ptr;
}


Processor* WorldInteractor::addProcessor(Block* pBlock, const caDraw::VectorF& dir)
{
	auto proc = std::make_unique<Processor>(pBlock, dir);

	Processor* ptr = proc.get();


	m_procs.emplace_back(std::move(proc));


	return ptr;
}

//#################################################################################################

void WorldInteractor::connectLinkerToNear(Block* pCenter)
{
	const auto boardSize = m_tileBoard.size();


	auto& port = pCenter->getLinkerPort();

	const auto& position = pCenter->getPosition();

	const caDraw::VectorT<size_t> tileCoord{
		static_cast<size_t>(position.x + 0.5f),
		static_cast<size_t>(position.y + 0.5f)
	};

	if (tileCoord.y > 0)
	{
		tryConnectLinkerTo(tileCoord.x, tileCoord.y - 1, pCenter, port);

		if (tileCoord.x > 0)
		{
			tryConnectLinkerTo(tileCoord.x - 1, tileCoord.y - 1, pCenter, port);
		}

		if (tileCoord.x < boardSize - 1)
		{
			tryConnectLinkerTo(tileCoord.x + 1, tileCoord.y - 1, pCenter, port);
		}
	}

	if (tileCoord.x > 0)
	{
		tryConnectLinkerTo(tileCoord.x - 1, tileCoord.y, pCenter, port);
	}

	if (tileCoord.x < boardSize - 1)
	{
		tryConnectLinkerTo(tileCoord.x + 1, tileCoord.y, pCenter, port);
	}

	if (tileCoord.y < boardSize - 1)
	{
		tryConnectLinkerTo(tileCoord.x, tileCoord.y + 1, pCenter, port);

		if (tileCoord.x > 0)
		{
			tryConnectLinkerTo(tileCoord.x - 1, tileCoord.y + 1, pCenter, port);
		}

		if (tileCoord.x < boardSize - 1)
		{
			tryConnectLinkerTo(tileCoord.x + 1, tileCoord.y + 1, pCenter, port);
		}
	}
}

//#################################################################################################

void WorldInteractor::tryConnectLinkerTo(std::size_t x, std::size_t y, Block* pCenter, LinkerPort& port)
{
	auto& nearBlocks = m_tileBoard[y][x]->getBlocks();

	if (nearBlocks.empty() == false)
	{
		auto* pTarget = nearBlocks[0];

		if (port.contains(pTarget) == false)
		{
			addLinker(*pCenter, *pTarget);
		}
	}
}

