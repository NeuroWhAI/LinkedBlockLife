#include "Block.h"




Block::Block()
	: m_position(0, 0)
	, m_speed(0, 0)
	, m_force(0, 0)
	, m_energy(DEFAULT_ENERGY)

	, m_linkerPort(*this)

	, m_data(0)
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

//-------------------------------------------------------------------------------------------------

int Block::getEnergy() const
{
	return m_energy;
}


void Block::setEnergy(int energy)
{
	m_energy = energy;
}


void Block::addEnergy(int energy)
{
	m_energy += energy;
}

//#################################################################################################

bool Block::willDisappear() const
{
	return (m_energy <= 0);
}


void Block::updateSpeed()
{
	m_speed += m_force;


	const float maxSpeed = 1.0f;
	const float lengthSq = m_speed.getLengthSq();

	if (lengthSq > maxSpeed/* * maxSpeed*/) // 1의 2승은 1이므로 제곱 연산을 제거.
	{
		// Normalize speed.
		m_speed *= maxSpeed / std::sqrt(lengthSq);
	}


	m_force = caDraw::VectorF::Zero;
}


void Block::updatePosition()
{
	m_position += m_speed;
}

//#################################################################################################

LinkerPort& Block::getLinkerPort()
{
	return m_linkerPort;
}

//#################################################################################################

int Block::getData() const
{
	return m_data;
}


void Block::setData(int data)
{
	m_data = data;
}

