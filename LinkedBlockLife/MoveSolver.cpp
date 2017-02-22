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

		// �ܷ� �ۿ�.
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
		auto halfSpeed = blockSpeed * 0.5f;
		nextTile->addOuterForce(halfSpeed);
		block.addForce(-halfSpeed);

		blockSpeed = caDraw::VectorF::Zero;
		tile.addNextBlock(&block);
	}
	else
	{
		// ���� ��ġ�� �ش��ϴ� Ÿ�Ͽ� �� ���.
		nextTile->addNextBlock(&block);
	}


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

