#include "Block.h"




Block::Block()
	: m_position(0, 0)
	, m_speed(0, 0)

	, m_linkerPort(*this)
{

}

//#################################################################################################

const caDraw::VectorF& Block::getPosition() const
{
	return m_position;
}


void Block::setPosition(const caDraw::VectorF& position)
{
	m_position = position;
}


void Block::addPosition(const caDraw::VectorF& position)
{
	m_position += position;
}

//-------------------------------------------------------------------------------------------------

const caDraw::VectorF& Block::getSpeed() const
{
	return m_speed;
}


void Block::setSpeed(const caDraw::VectorF& speed)
{
	m_speed = speed;
}


void Block::addSpeed(const caDraw::VectorF& speed)
{
	m_speed += speed;
}

//#################################################################################################

void Block::updatePosition()
{
	m_position += m_speed;
}

//#################################################################################################

LinkerPort& Block::getLinkerPort()
{
	return m_linkerPort;
}

