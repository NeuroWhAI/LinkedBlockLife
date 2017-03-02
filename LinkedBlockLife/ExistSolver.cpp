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
		* 아래 코드는 m_targetBlocks[1], m_targetBlocks[2], ..., m_targetBlocks[3], m_targetBlocks[0]
		* 순서에서 각 원소를 차례로 순회하였을때 값이 오름차순으로 되어있을때만
		* 정상적으로 동작한다.
		* 때문에 Solver에서 checkBlock을 호출하는 로직에 의존성을 가진다.
		*/

		for (std::size_t index = 0; index < blockCount; ++index)
		{
			// 목표가 있는 목록을 찾을때까지 탐색.
			// 끝까지 목표를 못찾았으면 currentCore는 0이고
			// currentItr == currentEnd인 상태가 된다.
			while (currentCore != 0 && currentItr == currentEnd)
			{
				++currentCore;
				if (currentCore >= coreCount)
					currentCore = 0;

				currentItr = m_targetBlocks[currentCore].cbegin();
				currentEnd = m_targetBlocks[currentCore].cend();
			}

			// 삭제할 목표가 아직 남아있고
			// 삭제할 목표를 찾았으면
			if ((currentCore != 0 || currentItr != currentEnd)
				&& *currentItr == index)
			{
				// 먼저 월드에서 제거.
				removeBlockFromWorld(*blocks[index], board);

				// 기존 목록에 저장하지 않고 다음 목표로 이동.
				++currentItr;
			}
			else
			{
				// 삭제할 목표가 아니면

				if (*pivot != blocks[index])
				{
					// 그대로 기존 목록에 저장.
					*pivot = std::move(blocks[index]);
				}

				++pivot;
			}
		}


		// 완벽히 제거.
		blocks.erase(pivot, blocks.end());


		// 목표 초기화.
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


	// TODO: 링커 끊기 및 링커 삭제.
}

