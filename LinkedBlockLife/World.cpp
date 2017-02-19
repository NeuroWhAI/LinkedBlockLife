#include "World.h"




World::World(std::size_t size)
	: m_solver(m_threadPool, m_board, m_blocks, m_linkers)
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
	m_blocks.emplace_back(std::make_unique<Block>());
	m_blocks.emplace_back(std::make_unique<Block>());
	m_blocks.emplace_back(std::make_unique<Block>());
	m_linkers.emplace_back(std::make_unique<Linker>(*m_blocks[0], *m_blocks[1]));
	m_linkers.emplace_back(std::make_unique<Linker>(*m_blocks[1], *m_blocks[2]));
	m_linkers.emplace_back(std::make_unique<Linker>(*m_blocks[2], *m_blocks[0]));

	m_blocks[0]->getLinkerPort().connect(m_linkers[0].get());
	m_blocks[1]->getLinkerPort().connect(m_linkers[0].get());

	m_blocks[1]->getLinkerPort().connect(m_linkers[1].get());
	m_blocks[2]->getLinkerPort().connect(m_linkers[1].get());

	m_blocks[2]->getLinkerPort().connect(m_linkers[2].get());
	m_blocks[0]->getLinkerPort().connect(m_linkers[2].get());

	m_blocks[0]->setPosition({ 20, 20 });
	m_blocks[1]->setPosition({ 21, 20 });
	m_blocks[2]->setPosition({ 22, 21 });
	m_blocks[2]->setForce({ 0.2f, 0 });
}

//#################################################################################################

void World::update()
{
	m_solver.solve();

	// Temp
	for (auto& block : m_blocks)
	{
		block->update();
	}
}

