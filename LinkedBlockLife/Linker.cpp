#include "Linker.h"

#include <cassert>
#include <algorithm>

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
	if (std::abs(m_length) > std::numeric_limits<f32>::epsilon())
	{
		m_dir /= m_length;
	}
	else
	{
		// TODO: Randomly.
		m_dir = { 1, 0 };
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

