#include "MoveSolver.h"

#include <random>

#include "CodeAdapter\EasyCA.h"

#include "Block.h"
#include "Tile.h"




using std::size_t;

//#################################################################################################

MoveSolver::MoveSolver()
{

}

//#################################################################################################

void MoveSolver::updateTile(Tile& tile, std::size_t x, std::size_t y)
{
	tile.updateBlockList();


	auto& blocks = tile.getBlocks();

	if (!blocks.empty())
	{
		const auto density = blocks.size();

		/// �� �� �̻� �����־ ��Ʈ���� �ϴ°�?
		const bool doSpread = (density >= 2);

		auto outerForce = tile.getOuterForce();
		outerForce /= static_cast<float>(density);

		for (auto* pBlock : blocks)
		{
			// �� �� �̻� �������� ��� Ÿ�� ������ �������� ����.
			if (doSpread)
			{
				auto pushVec = pBlock->getPosition();
				pushVec -= caDraw::VectorF{
					static_cast<float>(x),
					static_cast<float>(y)
				};

				const float lenSq = pushVec.getLengthSq();

				// ����ȭ.
				if (lenSq > std::numeric_limits<f32>::epsilon())
				{
					const float len = std::sqrt(lenSq);

					pushVec /= len;
				}
				else
				{
					std::random_device rd;
					std::uniform_int_distribution<> dist{ 1, 360 };

					const float rad = dist(rd) * 0.01745329251f;

					// Random direction.
					pushVec.x = std::cosf(rad);
					pushVec.y = std::sinf(rad);
				}

				pushVec *= 6.6742e-5f;

				pBlock->addForce(pushVec);
			}

			// �ܷ� �ۿ�.
			pBlock->addForce(outerForce);
		}
	}

	// �ܷ� �ʱ�ȭ.
	tile.setOuterForce(caDraw::VectorF::Zero);
}


void MoveSolver::updateBlock(TileBoard& board, std::size_t boardSize, Block& block, Tile& tile,
	std::size_t density)
{
	// ���� �ӵ��� ��ȯ.
	block.updateSpeed();


	auto blockSpeed = block.getSpeed();
	auto nextPosition = block.getPosition() + blockSpeed;


	// ��/������ ������� Ȯ��.
	// ��ȣ ���� �ڷ������� ĳ���� �Ұ��̹Ƿ� ������ ����.
	if (nextPosition.x < -0.5f)
	{
		nextPosition.x = -0.5f;

		if (blockSpeed.x < 0)
			blockSpeed.x = 0;
	}
	if (nextPosition.y < -0.5f)
	{
		nextPosition.y = -0.5f;

		if (blockSpeed.y < 0)
			blockSpeed.y = 0;
	}


	caDraw::VectorT<size_t> nextTileCoord{
		static_cast<size_t>(nextPosition.x + 0.5f),
		static_cast<size_t>(nextPosition.y + 0.5f)
	};


	// ��/�Ϸ� ������� Ȯ��.
	if (nextTileCoord.x >= boardSize)
	{
		nextTileCoord.x = boardSize - 1;

		if (blockSpeed.x > 0)
			blockSpeed.x = 0;
	}
	if (nextTileCoord.y >= boardSize)
	{
		nextTileCoord.y = boardSize - 1;

		if (blockSpeed.y > 0)
			blockSpeed.y = 0;
	}


	auto& nextTile = board[nextTileCoord.y][nextTileCoord.x];

	if (nextTile.get() != &tile && nextTile->isBlocked())
	{
		auto halfSpeed = blockSpeed * 0.8f;

		nextTile->addOuterForce(halfSpeed);
		block.addForce(-halfSpeed);
	}
	
	// ���� ��ġ�� �ش��ϴ� Ÿ�Ͽ� �� ���.
	nextTile->addNextBlock(&block);


	// ���� �б�.
	if (nextTile.get() != &tile)
	{
		float moveAir = tile.getPressure() / density * 0.82f;

		nextTile->addNextPressure(moveAir);
		tile.addNextPressure(-moveAir);
	}


	// �ӵ� �缳��.
	block.setSpeed(blockSpeed);


	// �ӵ��� ��ġ ����.
	block.updatePosition();
}

