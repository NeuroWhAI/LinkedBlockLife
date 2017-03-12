#include "World.h"




World::World(std::size_t size)
	: m_solver(m_threadPool, m_board, m_blocks, m_linkers, m_processors)
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

	addBlock(10, 40, 4);
	addBlock(11, 40, 4);
	addBlock(12, 40, 4);
	addBlock(13, 40, 1);
	addBlock(14, 40, 5);
	addBlock(15, 40, 5);
	addBlock(15, 40, 5);

	addLinker(*m_blocks[index], *m_blocks[index + 1]);
	addLinker(*m_blocks[index + 1], *m_blocks[index + 2]);
	addLinker(*m_blocks[index + 2], *m_blocks[index + 3]);
	addLinker(*m_blocks[index + 3], *m_blocks[index + 4]);
	addLinker(*m_blocks[index + 4], *m_blocks[index + 5]);
	addLinker(*m_blocks[index + 5], *m_blocks[index + 6]);

	addProcessor(m_blocks[index].get(), { 1, 0 });
}

//#################################################################################################

void World::update()
{
	m_solver.solve();
}

//#################################################################################################

Block* World::addBlock(std::size_t x, std::size_t y, int data)
{
	auto block = std::make_unique<Block>();

	Block* ptr = block.get();


	block->setPosition({ static_cast<float>(x), static_cast<float>(y) });
	block->setData(data);

	m_board[y][x]->addBlock(ptr);

	m_blocks.emplace_back(std::move(block));


	return ptr;
}


Linker* World::addLinker(Block& first, Block& second)
{
	auto linker = std::make_unique<Linker>(first, second);

	Linker* ptr = linker.get();


	first.getLinkerPort().connect(ptr);
	second.getLinkerPort().connect(ptr);


	m_linkers.emplace_back(std::move(linker));


	return ptr;
}


Processor* World::addProcessor(Block* pBlock, const caDraw::VectorF& dir)
{
	auto proc = std::make_unique<Processor>(pBlock, dir);

	Processor* ptr = proc.get();


	m_processors.emplace_back(std::move(proc));


	return ptr;
}

