#include "ExistSolver.h"

#include <cassert>

#include "Tile.h"
#include "Block.h"




using std::size_t;

//#################################################################################################

ExistSolver::ExistSolver(std::size_t coreCount)
	: m_needRemove(false)
{
	m_targetBlocks.resize(coreCount);
}

//#################################################################################################

void ExistSolver::checkBlock(std::size_t coreIndex, Block& block, std::size_t blockIndex)
{
	assert(coreIndex < m_targetBlocks.size());


	if (block.getEnergy() <= 0)
	{
		m_needRemove = true;

		m_targetBlocks[coreIndex].emplace_back(blockIndex);
	}
}


void ExistSolver::removeTargetBlocks(BlockList& blocks, TileBoard& board)
{
	if (m_needRemove)
	{
		const auto coreCount = m_targetBlocks.size();

		auto pivot = blocks.begin();
		const auto blockCount = blocks.size();

		std::size_t currentCore = ((coreCount >= 2) ? 1 : 0);
		auto currentItr = m_targetBlocks[currentCore].cbegin();
		auto currentEnd = m_targetBlocks[currentCore].cend();

		/*
		* �Ʒ� �ڵ�� m_targetBlocks[1], m_targetBlocks[2], ..., m_targetBlocks[3], m_targetBlocks[0]
		* �������� �� ���Ҹ� ���ʷ� ��ȸ�Ͽ����� ���� ������������ �Ǿ���������
		* ���������� �����Ѵ�.
		* ������ Solver���� checkBlock�� ȣ���ϴ� ������ �������� ������.
		*/

		for (std::size_t index = 0; index < blockCount; ++index)
		{
			// ��ǥ�� �ִ� ����� ã�������� Ž��.
			// ������ ��ǥ�� ��ã������ currentCore�� 0�̰�
			// currentItr == currentEnd�� ���°� �ȴ�.
			while (currentCore != 0 && currentItr == currentEnd)
			{
				++currentCore;
				if (currentCore >= coreCount)
					currentCore = 0;

				currentItr = m_targetBlocks[currentCore].cbegin();
				currentEnd = m_targetBlocks[currentCore].cend();
			}

			// ������ ��ǥ�� ���� �����ְ�
			// ������ ��ǥ�� ã������
			if ((currentCore != 0 || currentItr != currentEnd)
				&& *currentItr == index)
			{
				// ���� ���忡�� ����.
				removeBlockFromWorld(*blocks[index], board);

				// ���� ��Ͽ� �������� �ʰ� ���� ��ǥ�� �̵�.
				++currentItr;
			}
			else
			{
				// ������ ��ǥ�� �ƴϸ�

				if (*pivot != blocks[index])
				{
					// �״�� ���� ��Ͽ� ����.
					*pivot = std::move(blocks[index]);
				}

				++pivot;
			}
		}


		// �Ϻ��� ����.
		blocks.erase(pivot, blocks.end());


		// ��ǥ �ʱ�ȭ.
		for (auto& ls : m_targetBlocks)
		{
			ls.clear();
		}

		m_needRemove = false;
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


	// TODO: ��Ŀ ���� �� ��Ŀ ����.
}

