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

		/// 두 블럭 이상 뭉쳐있어서 퍼트려야 하는가?
		const bool doSpread = (density >= 2);

		auto outerForce = tile.getOuterForce();
		outerForce /= static_cast<float>(density);

		for (auto* pBlock : blocks)
		{
			// 두 블럭 이상 뭉쳐있을 경우 타일 밖으로 나가도록 가속.
			if (doSpread)
			{
				auto pushVec = pBlock->getPosition();
				pushVec -= caDraw::VectorF{
					static_cast<float>(x),
					static_cast<float>(y)
				};

				const float lenSq = pushVec.getLengthSq();

				// 정규화.
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

			// 외력 작용.
			pBlock->addForce(outerForce);
		}
	}

	// 외력 초기화.
	tile.setOuterForce(caDraw::VectorF::Zero);
}


void MoveSolver::updateBlock(TileBoard& board, std::size_t boardSize, Block& block, Tile& tile,
	std::size_t density)
{
	// 힘을 속도로 변환.
	block.updateSpeed();


	auto blockSpeed = block.getSpeed();
	auto nextPosition = block.getPosition() + blockSpeed;


	// 좌/상으로 벗어났는지 확인.
	// 부호 없는 자료형으로 캐스팅 할것이므로 음수를 제거.
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


	// 우/하로 벗어났는지 확인.
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
	
	// 다음 위치에 해당하는 타일에 블럭 등록.
	nextTile->addNextBlock(&block);


	// 공기 밀기.
	if (nextTile.get() != &tile)
	{
		float moveAir = tile.getPressure() / density * 0.82f;

		nextTile->addNextPressure(moveAir);
		tile.addNextPressure(-moveAir);
	}


	// 속도 재설정.
	block.setSpeed(blockSpeed);


	// 속도로 위치 갱신.
	block.updatePosition();
}

