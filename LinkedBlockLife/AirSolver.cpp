#include "AirSolver.h"

#include "ThreadPool.h"
#include "Tile.h"




using std::size_t;

//#################################################################################################

AirSolver::AirSolver(TileBoard& board, ThreadPool& threadPool)
	: m_board(board)
	, m_threadPool(threadPool)
{
	
}

//#################################################################################################

void AirSolver::solve()
{
	const size_t boardSize = m_board.size();

	const size_t coreCount = std::thread::hardware_concurrency();
	const size_t heightPerCore = boardSize / coreCount;


	std::vector<std::future<void>> futList;
	futList.reserve(coreCount - 1);


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&AirSolver::updateRange, this,
			core * heightPerCore + 2, heightPerCore - 2);

		futList.emplace_back(std::move(fut));
	}

	updateRange(2, heightPerCore - 2);

	for (auto& fut : futList)
	{
		fut.wait();
	}


	futList.clear();


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&AirSolver::updateRange, this,
			core * heightPerCore, 2);

		futList.emplace_back(std::move(fut));
	}

	updateRange(0, 2);

	for (auto& fut : futList)
	{
		fut.wait();
	}


	futList.clear();


	for (size_t core = 1; core < coreCount; ++core)
	{
		auto fut = m_threadPool.reserve(&AirSolver::updateTileRange, this,
			core * heightPerCore, heightPerCore);

		futList.emplace_back(std::move(fut));
	}

	updateTileRange(0, heightPerCore);

	for (auto& fut : futList)
	{
		fut.wait();
	}
}

//#################################################################################################

void AirSolver::updateRange(std::size_t beginY, std::size_t count)
{
	constexpr float oneRootTwo = 1.0f / 1.4142135623730950488016887242097f;

	const size_t boardSize = m_board.size();

	const size_t endY = std::min(beginY + count, boardSize);

	for (size_t y = beginY; y < endY; ++y)
	{
		auto& line = m_board[y];

		for (size_t x = 0; x < boardSize; ++x)
		{
			auto& here = line[x];

			auto norWindHere = here->getWind();
			norWindHere.normalize();

			caDraw::VectorF dirVec[] = {
				{ 0, -1 },
				{ 1, 0 },
				{ 0, 1 },
				{ -1, 0 },
				{ -oneRootTwo, -oneRootTwo },
				{ oneRootTwo, -oneRootTwo },
				{ oneRootTwo, oneRootTwo },
				{ -oneRootTwo, oneRootTwo },
			};

			const size_t ySubOne = (y <= 0) ? boardSize : y - 1;
			const size_t xSubOne = (x <= 0) ? boardSize : x - 1;

			size_t nearY[] = {
				ySubOne, y, y + 1, y,
				ySubOne, ySubOne, y + 1, y + 1
			};
			size_t nearX[] = {
				x, x + 1, x, xSubOne,
				xSubOne, x + 1, x + 1, xSubOne
			};

			for (int n = 0; n < 8; ++n)
			{
				if (nearY[n] >= boardSize)
					continue;
				else if (nearX[n] >= boardSize)
					continue;

				auto& near = m_board[nearY[n]][nearX[n]];

				if (near->isBlocked() == false)
				{
					float airGap = here->getPressure() - near->getPressure();

					if (airGap > 0)
					{
						float spreadAir = airGap / 2 * 0.06f;

						near->addNextPressure(spreadAir);
						here->addNextPressure(-spreadAir);

						near->addNextWind(dirVec[n] * (spreadAir * 0.4f));
					}
				}

				float dot = norWindHere.dotProduct(dirVec[n]);

				if (dot > 0)
				{
					float scale = dot * dot * dot * 0.08f;

					auto moveWind = here->getWind() * scale;

					if (near->isBlocked())
					{
						here->addNextWind(-moveWind);
					}
					else
					{
						near->addNextWind(moveWind);
						here->addNextWind(-moveWind);

						float moveAir = here->getPressure() * scale;

						near->addNextPressure(moveAir);
						here->addNextPressure(-moveAir);
					}
				}
			}
		}
	}
}


void AirSolver::updateTileRange(std::size_t beginY, std::size_t count)
{
	const size_t boardSize = m_board.size();
	const size_t endY = std::min(beginY + count, boardSize);

	for (size_t y = beginY; y < endY; ++y)
	{
		auto& line = m_board[y];

		for (auto& tile : line)
		{
			tile->updateAir();
		}
	}
}

