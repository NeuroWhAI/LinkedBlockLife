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
	addBlock(std::make_unique<Block>(), 10, 20);
	addBlock(std::make_unique<Block>(), 11, 20);
	addBlock(std::make_unique<Block>(), 12, 21);
	m_linkers.emplace_back(std::make_unique<Linker>(*m_blocks[0], *m_blocks[1]));
	m_linkers.emplace_back(std::make_unique<Linker>(*m_blocks[1], *m_blocks[2]));
	m_linkers.emplace_back(std::make_unique<Linker>(*m_blocks[2], *m_blocks[0]));

	m_blocks[0]->getLinkerPort().connect(m_linkers[0].get());
	m_blocks[1]->getLinkerPort().connect(m_linkers[0].get());

	m_blocks[1]->getLinkerPort().connect(m_linkers[1].get());
	m_blocks[2]->getLinkerPort().connect(m_linkers[1].get());

	m_blocks[2]->getLinkerPort().connect(m_linkers[2].get());
	m_blocks[0]->getLinkerPort().connect(m_linkers[2].get());

	m_blocks[0]->setForce({ 1.0f, 0 });
	m_blocks[1]->setForce({ 1.0f, 0 });
	m_blocks[2]->setForce({ 1.0f, 0 });

	for (int y = 0; y < 15; ++y)
	{
		for (int x = 0; x < 30; ++x)
		{
			auto index = addBlock(std::make_unique<Block>(), 20 + x, 15 + y);
		}
	}
}

//#################################################################################################

void World::update()
{
	m_solver.solve();
}

//#################################################################################################

std::size_t World::addBlock(std::unique_ptr<Block> block, std::size_t x, std::size_t y)
{
	auto index = m_blocks.size();


	block->setPosition({ static_cast<float>(x), static_cast<float>(y) });

	m_board[y][x]->addBlock(block.get());

	m_blocks.emplace_back(std::move(block));


	return index;
}

