#include "AirSolver.h"

#include "Tile.h"




using std::size_t;

//#################################################################################################

AirSolver::AirSolver()
{
	
}

//#################################################################################################

void AirSolver::updateNearTile(TileBoard& board, Tile& here, std::size_t x, std::size_t y)
{
	constexpr float oneRootTwo = 1.0f / 1.4142135623730950488016887242097f;

	const auto boardSize = board.size();


	auto norWindHere = here.getWind();
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


		auto& near = board[nearY[n]][nearX[n]];


		if (near->isBlocked() == false)
		{
			float airGap = here.getPressure() - near->getPressure();

			if (airGap > 0)
			{
				float spreadAir = airGap / 2 * 0.06f;

				near->addNextPressure(spreadAir);
				here.addNextPressure(-spreadAir);

				near->addNextWind(dirVec[n] * (spreadAir * 0.4f));
			}
		}


		float dot = norWindHere.dotProduct(dirVec[n]);

		if (dot > 0)
		{
			float scale = dot * dot * dot * 0.08f;

			auto moveWind = here.getWind() * scale;

			if (near->isBlocked())
			{
				here.addNextWind(-moveWind);
			}
			else
			{
				near->addNextWind(moveWind);
				here.addNextWind(-moveWind);

				float moveAir = here.getPressure() * scale;

				near->addNextPressure(moveAir);
				here.addNextPressure(-moveAir);
			}
		}
	}
}


void AirSolver::updateTile(Tile& tile)
{
	tile.updateAir();
}

