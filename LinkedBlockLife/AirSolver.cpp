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


	/// �ܷ�
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


		// �ܷ� ���
		if (dirVec[n].dotProduct(windNear) < 0)
		{
			outerForce += windNear * 0.001f;
		}

		outerForce += dirVec[n] * (-pressureNear * 0.0001f);


		// ���� ���� �ƴϸ� ������� �̿��Ͽ� ����� �̵��� �ٶ��� ����Ѵ�.
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

		// �ٶ��� �δ� ���⿡ �����ϴ� Ÿ������ Ȯ��.
		if (dot > 0)
		{
			/// �ٶ��� ���⿡ ����� ����.
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


	// �ܷ� ����.
	here.addOuterForce(outerForce);
}


void AirSolver::updateTile(Tile& tile, std::size_t boardSize, std::size_t x, std::size_t y)
{
	// �׵θ��� �ٶ��� �����ǰ� �Ҹ����� �ʴ� ���� �ذ�.
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

