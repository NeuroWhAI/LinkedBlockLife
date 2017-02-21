#include "Tile.h"




Tile::Tile()
	: m_pBlocks(&m_blocks1)
	, m_pNextBlocks(&m_blocks2)
	
	, m_wind(0, 0)
	, m_nextWind(0, 0)
	, m_pressure(100)
	, m_nextPressure(100)
{

}

//#################################################################################################

bool Tile::isBlocked() const
{
	return (!m_pBlocks->empty());
}


const std::vector<Block*>& Tile::getBlocks() const
{
	return *m_pBlocks;
}


void Tile::addBlock(Block* pBlock)
{
	m_pBlocks->emplace_back(pBlock);
}


const std::vector<Block*>& Tile::getNextBlocks() const
{
	return *m_pNextBlocks;
}


void Tile::addNextBlock(Block* pBlock)
{
	m_pNextBlocks->emplace_back(pBlock);
}

//#################################################################################################

const caDraw::VectorF& Tile::getWind() const
{
	return m_wind;
}


const caDraw::VectorF& Tile::getNextWind() const
{
	return m_nextWind;
}


void Tile::setNextWind(const caDraw::VectorF& wind)
{
	m_nextWind = wind;
}


void Tile::addNextWind(const caDraw::VectorF& wind)
{
	m_nextWind += wind;
}

//-------------------------------------------------------------------------------------------------

float Tile::getPressure() const
{
	return m_pressure;
}


float Tile::getNextPressure() const
{
	return m_nextPressure;
}


void Tile::setNextPressure(float pressure)
{
	m_nextPressure = pressure;
}


void Tile::addNextPressure(float pressure)
{
	m_nextPressure += pressure;
}

//#################################################################################################

void Tile::updateBlockList()
{
	std::swap(m_pBlocks, m_pNextBlocks);
	m_pNextBlocks->clear();
}


void Tile::updateAir()
{
	m_wind = m_nextWind;
	m_pressure = m_nextPressure;

	m_nextWind = m_wind * 0.9999f;
}

