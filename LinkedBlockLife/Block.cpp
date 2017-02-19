#include "Block.h"




Block::Block()
	: m_position(0, 0)
	, m_speed(0, 0)
	, m_force(0, 0)

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

//-------------------------------------------------------------------------------------------------

const caDraw::VectorF& Block::getForce() const
{
	return m_force;
}


void Block::setForce(const caDraw::VectorF& force)
{
	m_force = force;
}


void Block::addForce(const caDraw::VectorF& force)
{
	m_force += force;
}

//#################################################################################################

void Block::update()
{
	m_speed += m_force / 1.0f/*mass*/;
	m_position += m_speed;

	m_force = caDraw::VectorF::Zero;
}

//#################################################################################################

LinkerPort& Block::getLinkerPort()
{
	return m_linkerPort;
}

