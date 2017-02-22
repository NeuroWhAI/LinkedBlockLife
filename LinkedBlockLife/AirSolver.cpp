#include "AirSolver.h"

#include "Tile.h"
#include "Block.h"




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


	const float pressureHere = here.getPressure();

	auto norWindHere = here.getWind();
	norWindHere.normalize();


	const caDraw::VectorF dirVec[] = {
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


	/// 외력
	caDraw::VectorF outerForce{ 0, 0 };


	for (int n = 0; n < 8; ++n)
	{
		if (nearY[n] >= boardSize)
			continue;
		else if (nearX[n] >= boardSize)
			continue;


		auto& near = board[nearY[n]][nearX[n]];
		auto& windNear = near->getWind();
		const auto pressureNear = near->getPressure();


		// 외력 계산
		if (dirVec[n].dotProduct(windNear) < 0)
		{
			outerForce += windNear * 0.001f;
		}

		outerForce += dirVec[n] * (-pressureNear * 0.0001f);


		// 막힌 블럭이 아니면 기압차를 이용하여 기압의 이동과 바람을 계산한다.
		if (near->isBlocked() == false)
		{
			float airGap = pressureHere - pressureNear;

			if (airGap > 0)
			{
				float spreadAir = airGap / 2 * 0.06f;

				near->addNextPressure(spreadAir);
				here.addNextPressure(-spreadAir);

				near->addNextWind(dirVec[n] * (spreadAir * 0.4f));
			}
		}


		float dot = norWindHere.dotProduct(dirVec[n]);

		// 바람이 부는 방향에 존재하는 타일인지 확인.
		if (dot > 0)
		{
			/// 바람의 방향에 가까운 정도.
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

				float moveAir = pressureHere * scale;

				near->addNextPressure(moveAir);
				here.addNextPressure(-moveAir);
			}
		}
	}


	// 외력 누적.
	here.addOuterForce(outerForce);
}


void AirSolver::updateTile(Tile& tile, std::size_t boardSize, std::size_t x, std::size_t y)
{
	// 테두리에 바람이 누적되고 소멸하지 않는 문제 해결.
	if (x == 0 || x == boardSize - 1)
	{
		tile.setNextWind({ 0, tile.getNextWind().y * 0.99f });
	}
	if (y == 0 || y == boardSize - 1)
	{
		tile.setNextWind({ tile.getNextWind().x * 0.99f, 0 });
	}


	tile.updateAir();
}

