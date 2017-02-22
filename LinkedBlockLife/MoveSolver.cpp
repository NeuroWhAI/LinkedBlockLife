#include "MoveSolver.h"

#include "CodeAdapter\EasyCA.h"

#include "Block.h"
#include "Tile.h"




using std::size_t;

//#################################################################################################

MoveSolver::MoveSolver()
{

}

//#################################################################################################

void MoveSolver::updateTile(Tile& tile)
{
	auto& blocks = tile.getBlocks();

	if (!blocks.empty())
	{
		auto outerForce = tile.getOuterForce();
		outerForce /= static_cast<float>(blocks.size());

		// 외력 작용.
		for (auto* pBlock : blocks)
		{
			pBlock->addForce(outerForce);
		}
	}

	tile.setOuterForce({ 0, 0 });


	tile.updateBlockList();
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
		auto halfSpeed = blockSpeed * 0.5f;
		nextTile->addOuterForce(halfSpeed);
		block.addForce(-halfSpeed);

		blockSpeed = caDraw::VectorF::Zero;
		tile.addNextBlock(&block);
	}
	else
	{
		// 다음 위치에 해당하는 타일에 블럭 등록.
		nextTile->addNextBlock(&block);
	}


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

