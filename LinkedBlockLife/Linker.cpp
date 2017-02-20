#include "Linker.h"

#include <cassert>
#include <algorithm>
#include <random>

#include "Block.h"




Linker::Linker(Block& first, Block& second)
	: m_defaultLength(1.0f)
	, m_dir(0, 0)
	, m_length(0)

	, m_first(first)
	, m_second(second)
{
	assert(&first != &second);
}

//#################################################################################################

Block& Linker::getFirst()
{
	return m_first;
}


Block& Linker::getSecond()
{
	return m_second;
}

//#################################################################################################

void Linker::calculateDir()
{
	m_dir = m_second.getPosition() - m_first.getPosition();
	m_length = m_dir.getLength();

	// Normalize vector.
	if (m_length > std::numeric_limits<f32>::epsilon())
	{
		m_dir /= m_length;
	}
	else
	{
		std::random_device rd;
		std::uniform_int_distribution<> dist{ 1, 360 };

		// Random direction.
		m_dir = { std::cosf(dist(rd) * 0.01745329251f), std::sinf(dist(rd) * 0.01745329251f) };
		m_length = 1.0f;
	}
}


caDraw::VectorF Linker::calculateElasticity() const
{
	/*
	* Thanks to [https://github.com/pmneila/jsexp/blob/master/massspring/particles.js].
	*/


	float lenGap = m_length - m_defaultLength;
	float dot = m_dir.dotProduct(m_first.getSpeed() - m_second.getSpeed());


	return (m_dir * (lenGap * 0.05f/*stiffness*/ - 0.2f/*damping*/ * dot));
}

