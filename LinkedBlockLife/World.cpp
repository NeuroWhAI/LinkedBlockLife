#include "World.h"




World::World(std::size_t size)
	: m_solver(m_threadPool, m_interactor, m_board, m_blocks, m_linkers, m_processors)
	, m_interactor(m_board, m_blocks, m_linkers, m_processors)
{
	m_board.resize(size);

	for (auto& line : m_board)
	{
		line.resize(size);

		for (auto& tile : line)
		{
			tile = std::make_unique<Tile>();
		}
	}

	// Test
	auto index = m_blocks.size();

	for (float degree = 0; degree < 360; degree += 8.0f)
	{
		float x = 32.0f + -cosf(degree * 0.0174533f) * 8.0f;
		float y = 20.0f + sinf(degree * 0.0174533f) * 8.0f;

		m_interactor.addBlock({ x, y }, 2)->setEnergy(Block::DEFAULT_ENERGY * 20);

		if (index > 0)
		{
			addLinker(*m_blocks[index - 1], *m_blocks[index]);
		}

		++index;
	}

	addLinker(*m_blocks[index - 1], *m_blocks[0]);

	index = m_blocks.size();

	addBlock(24, 12, 6);
	addBlock(24, 13, 6);
	addBlock(24, 14, 6);
	addBlock(24, 15, 6);
	addBlock(24, 16, 6);
	addBlock(24, 17, 6);
	addBlock(24, 18, 6);
	addBlock(24, 19, 6);

	addLinker(*m_blocks[index], *m_blocks[index + 1]);
	addLinker(*m_blocks[index + 1], *m_blocks[index + 2]);
	addLinker(*m_blocks[index + 2], *m_blocks[index + 3]);
	addLinker(*m_blocks[index + 3], *m_blocks[index + 4]);
	addLinker(*m_blocks[index + 4], *m_blocks[index + 5]);
	addLinker(*m_blocks[index + 5], *m_blocks[index + 6]);
	addLinker(*m_blocks[index + 6], *m_blocks[index + 7]);
	addLinker(*m_blocks[index + 7], *m_blocks[0]);

	addProcessor(m_blocks[index].get(), { 1, 0 });

	index = m_blocks.size();

	addBlock(10, 20);
	addBlock(11, 20);
	addBlock(12, 21);
	addLinker(*m_blocks[index], *m_blocks[index + 1]);
	addLinker(*m_blocks[index + 1], *m_blocks[index + 2]);
	addLinker(*m_blocks[index + 2], *m_blocks[index]);

	addProcessor(m_blocks[index].get(), { 1, 0 });

	index = m_blocks.size();

	addBlock(10, 40, 6);
	addBlock(11, 40, 6);
	addBlock(12, 40, 6);
	addBlock(13, 40, 2);
	addBlock(14, 40, 7);
	addBlock(15, 40, 7);
	addBlock(15, 40, 7);

	addLinker(*m_blocks[index], *m_blocks[index + 1]);
	addLinker(*m_blocks[index + 1], *m_blocks[index + 2]);
	addLinker(*m_blocks[index + 2], *m_blocks[index + 3]);
	addLinker(*m_blocks[index + 3], *m_blocks[index + 4]);
	addLinker(*m_blocks[index + 4], *m_blocks[index + 5]);
	addLinker(*m_blocks[index + 5], *m_blocks[index + 6]);

	addProcessor(m_blocks[index].get(), { 1, 0 });

	index = m_blocks.size();

	addBlock(30, 40, 6);
	addBlock(31, 40, 6);
	addBlock(32, 40, 6);
	addBlock(33, 40, 6);
	addBlock(34, 40, 6);
	addBlock(35, 40, 6);
	addBlock(36, 40, 2);
	addBlock(37, 40, 7);

	addLinker(*m_blocks[index], *m_blocks[index + 1]);
	addLinker(*m_blocks[index + 1], *m_blocks[index + 2]);
	addLinker(*m_blocks[index + 2], *m_blocks[index + 3]);
	addLinker(*m_blocks[index + 3], *m_blocks[index + 4]);
	addLinker(*m_blocks[index + 4], *m_blocks[index + 5]);
	addLinker(*m_blocks[index + 5], *m_blocks[index + 6]);
	addLinker(*m_blocks[index + 6], *m_blocks[index + 7]);

	addProcessor(m_blocks[index].get(), { 1, 0 });

	addBlock(35, 39);
	addBlock(36, 39);
	addBlock(37, 39);
	addBlock(35, 41);
	addBlock(36, 41);
	addBlock(37, 41);

	index = m_blocks.size();

	addBlock(10, 50, 6);
	addBlock(11, 50, 6);
	addBlock(12, 50, 6);
	addBlock(13, 50, 6);
	addBlock(14, 50, 6);
	addBlock(15, 50, 6);
	addBlock(16, 50, 6);
	addBlock(17, 50, 6);
	addBlock(18, 50, 2)->addEnergy(Block::DEFAULT_ENERGY * 10);
	addBlock(19, 50, 0);

	addLinker(*m_blocks[index], *m_blocks[index + 1]);
	addLinker(*m_blocks[index + 1], *m_blocks[index + 2]);
	addLinker(*m_blocks[index + 2], *m_blocks[index + 3]);
	addLinker(*m_blocks[index + 3], *m_blocks[index + 4]);
	addLinker(*m_blocks[index + 4], *m_blocks[index + 5]);
	addLinker(*m_blocks[index + 5], *m_blocks[index + 6]);
	addLinker(*m_blocks[index + 6], *m_blocks[index + 7]);
	addLinker(*m_blocks[index + 7], *m_blocks[index + 8]);
	addLinker(*m_blocks[index + 8], *m_blocks[index + 9]);

	addProcessor(m_blocks[index].get(), { 1, 0 });
}

//#################################################################################################

std::size_t World::getBoardSize() const
{
	return m_board.size();
}

//#################################################################################################

void World::update()
{
	m_solver.solve();
}

//#################################################################################################

Block* World::addBlock(std::size_t x, std::size_t y, int data)
{
	return m_interactor.addBlock(x, y, data);
}


Linker* World::addLinker(Block& first, Block& second)
{
	return m_interactor.addLinker(first, second);
}


Processor* World::addProcessor(Block* pBlock, const caDraw::VectorF& dir)
{
	return m_interactor.addProcessor(pBlock, dir);
}

