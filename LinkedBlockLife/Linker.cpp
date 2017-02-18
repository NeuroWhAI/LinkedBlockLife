#include "Linker.h"

#include <cassert>

#include "Block.h"




Linker::Linker(Block& first, Block& second)
	: m_defaultLength(1.0f)
	
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

caDraw::VectorF Linker::calculateElasticity() const
{
	auto subVec = m_first.getPosition() - m_second.getPosition();
	float length = subVec.getLength();

	// normalize subVec.
	if (std::abs(length) > std::numeric_limits<f32>::epsilon())
	{
		subVec /= length;
	}
	else
	{
		subVec = { 1, 0 };
	}

	float lenGap = length - m_defaultLength;


	return (subVec * (lenGap * 0.2f));
}

