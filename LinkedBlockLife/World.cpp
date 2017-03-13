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
	addBlock(10, 20);
	addBlock(11, 20);
	addBlock(12, 21);
	addLinker(*m_blocks[0], *m_blocks[1]);
	addLinker(*m_blocks[1], *m_blocks[2]);
	addLinker(*m_blocks[2], *m_blocks[0]);

	m_blocks[0]->setForce({ 1.0f, 0 });
	m_blocks[1]->setForce({ 1.0f, 0 });
	m_blocks[2]->setForce({ 1.0f, 0 });

	addProcessor(m_blocks[0].get(), { 1, 0 });

	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 30; ++x)
		{
			addBlock(20 + x, 15 + y);
		}
	}

	auto index = m_blocks.size();

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

