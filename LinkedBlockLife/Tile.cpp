#include "Tile.h"




Tile::Tile()
	: m_pBlock(nullptr)

	, m_wind(0, 0)
	, m_nextWind(0, 0)
	, m_pressure(10)
	, m_nextPressure(10)
{

}

//#################################################################################################

bool Tile::isBlocked() const
{
	return (m_pBlock != nullptr);
}


Block* Tile::getBlock() const
{
	return m_pBlock;
}


void Tile::setBlock(Block* pBlock)
{
	m_pBlock = pBlock;
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

void Tile::updateAir()
{
	m_wind = m_nextWind;
	m_pressure = m_nextPressure;

	m_wind *= 0.9999f;

	m_nextWind = m_wind;
	m_nextPressure = m_pressure;
}

