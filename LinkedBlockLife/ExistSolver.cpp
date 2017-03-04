#include "ExistSolver.h"

#include <cassert>

#include "Tile.h"
#include "Block.h"
#include "Linker.h"




using std::size_t;

//#################################################################################################

ExistSolver::ExistSolver(std::size_t coreCount)
	: m_currentCore(0)
	
	, m_needRemoveBlock(false)
	, m_needRemoveLinker(false)
{
	m_targetBlocks.resize(coreCount);
	m_targetLinkers.resize(coreCount);
}

//#################################################################################################

void ExistSolver::updateIterator(IndexBoard& indexBoard)
{
	const auto coreCount = indexBoard.size();

	// ��ǥ�� �ִ� ����� ã�������� Ž��.
	// ������ ��ǥ�� ��ã������ currentCore�� 0�̰�
	// currentItr == currentEnd�� ���°� �ȴ�.
	while (m_currentCore != 0 && m_currentItr == m_currentEnd)
	{
		++m_currentCore;
		if (m_currentCore >= coreCount)
			m_currentCore = 0;

		m_currentItr = indexBoard[m_currentCore].cbegin();
		m_currentEnd = indexBoard[m_currentCore].cend();
	}
}

//#################################################################################################

void ExistSolver::checkBlock(std::size_t coreIndex, Block& block, std::size_t blockIndex)
{
	assert(coreIndex < m_targetBlocks.size());


	if (block.getEnergy() <= 0)
	{
		m_needRemoveBlock = true;

		m_targetBlocks[coreIndex].emplace_back(blockIndex);
	}
}


void ExistSolver::removeTargetBlocks(BlockList& blocks, TileBoard& board)
{
	if (m_needRemoveBlock)
	{
		removeTarget(blocks, m_targetBlocks, &ExistSolver::removeBlockFromWorld, board);

		m_needRemoveBlock = false;
	}
}

//-------------------------------------------------------------------------------------------------

void ExistSolver::checkLinker(std::size_t coreIndex, Linker& linker, std::size_t linkerIndex)
{
	assert(coreIndex < m_targetLinkers.size());


	if (linker.isDisconnected())
	{
		m_needRemoveLinker = true;

		m_targetLinkers[coreIndex].emplace_back(linkerIndex);
	}
}


void ExistSolver::removeTargetLinkers(LinkerList& linkers)
{
	if (m_needRemoveLinker)
	{
		removeTarget(linkers, m_targetLinkers, &ExistSolver::removeLinkerFromWorld);

		m_needRemoveLinker = false;
	}
}

//#################################################################################################

void ExistSolver::removeBlockFromWorld(Block& block, TileBoard& board)
{
	auto& position = block.getPosition();

	caDraw::VectorT<size_t> tileCoord{
		static_cast<size_t>(position.x + 0.5f),
		static_cast<size_t>(position.y + 0.5f)
	};

	board[tileCoord.y][tileCoord.x]->removeBlock(&block);


	auto& linkerPort = block.getLinkerPort();
	auto& linkers = linkerPort.getLinkerList();
	auto& others = linkerPort.getTargetList();

	// ��Ŀ�� �ݴ��� ���� ���� ����
	for (auto* pOther : others)
	{
		pOther->getLinkerPort().disconnect(&block);
	}

	// ��Ŀ ���� ����.
	for (auto* pLinker : linkers)
	{
		pLinker->setDisconnectionFlag();
	}
}


void ExistSolver::removeLinkerFromWorld(Linker& linker)
{
	// NOTE: ������ �� �� ����.
}

