#include "World.h"




World::World(std::size_t size)
	: m_airSolver(m_board, m_threadPool)
	, m_linkSolver(m_blocks, m_threadPool)
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
}

//#################################################################################################

void World::update()
{
	m_airSolver.solve();
	m_linkSolver.solve();
}

