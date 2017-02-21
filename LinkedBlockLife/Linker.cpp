#include "Linker.h"

#include <cassert>
#include <algorithm>
#include <random>

#include "Block.h"




Linker::Linker(Block& first, Block& second)
	: m_defaultLength(1.0f)
	, m_elasticity(0, 0)

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

void Linker::calculateElasticity()
{
	/*
	* Thanks to [https://github.com/pmneila/jsexp/blob/master/massspring/particles.js].
	*/


	auto dir = m_second.getPosition() - m_first.getPosition();
	float length = dir.getLength();

	// Normalize vector.
	if (length > std::numeric_limits<f32>::epsilon())
	{
		dir /= length;
	}
	else
	{
		std::random_device rd;
		std::uniform_int_distribution<> dist{ 1, 360 };

		float rad = dist(rd) * 0.01745329251f;

		// Random direction.
		dir = { std::cosf(rad), std::sinf(rad) };
		length = 1.0f;
	}

	float lenGap = length - m_defaultLength;
	float dot = dir.dotProduct(m_first.getSpeed() - m_second.getSpeed());


	m_elasticity = dir * (lenGap * 0.05f/*stiffness*/ - 0.2f/*damping*/ * dot);
}


const caDraw::VectorF& Linker::getElasticity() const
{
	return m_elasticity;
}

