#include "Processor.h"

#include <cassert>
#include <numeric>

#include "Block.h"




Processor::Processor(Block* pBlock, const caDraw::VectorF& dir)
	: m_willDie(false)
	, m_pBlock(pBlock)
	, m_dir(dir)

	, m_ram(RAM_SIZE, 0)
	, m_ptr(0)
{
	m_cmdList.emplace_back(nullptr);
	m_cmdList.emplace_back(&Processor::cmdDoJob);
	m_cmdList.emplace_back(&Processor::cmdPtr2Left);
	m_cmdList.emplace_back(&Processor::cmdPtr2Right);
	m_cmdList.emplace_back(&Processor::cmdInc);
	m_cmdList.emplace_back(&Processor::cmdDec);

	m_jobList.emplace_back(nullptr);
	m_jobList.emplace_back(&Processor::jobReadNear);
}

//#################################################################################################

bool Processor::willDisappear() const
{
	return m_willDie;
}


void Processor::setBlock(Block* pBlock)
{
	m_pBlock = pBlock;
}


Block* Processor::getBlock()
{
	return m_pBlock;
}

//#################################################################################################

void Processor::execute()
{
	assert(m_pBlock != nullptr);


	// 현재 블럭의 명령어 실행.
	auto cmd = m_pBlock->getData();

	if (cmd > 0 && static_cast<std::size_t>(cmd) < m_cmdList.size())
	{
		(this->*m_cmdList[cmd])();
	}


	moveToNextBlock();
}

//#################################################################################################

void Processor::moveToNextBlock()
{
	auto& centerPos = m_pBlock->getPosition();

	float maxScore = std::numeric_limits<float>::lowest();
	auto nextDir = m_dir;

	auto& nearBlocks = m_pBlock->getLinkerPort().getTargetList();

	for (auto* pNearBlock : nearBlocks)
	{
		auto subVec = pNearBlock->getPosition() - centerPos;

		float score = m_dir.dotProduct(subVec);

		if (score > maxScore)
		{
			maxScore = score;
			m_pBlock = pNearBlock;
			nextDir = subVec;
		}
	}

	m_dir = nextDir;


	m_willDie = m_pBlock->willDisappear();
}

//#################################################################################################

void Processor::cmdDoJob()
{
	// 현재 포인터가 위치한 셀의 값에 해당하는 작업 수행.
	auto job = m_ram[m_ptr];

	if (job > 0 && static_cast<std::size_t>(job) < m_jobList.size())
	{
		(this->*m_jobList[job])();
	}
}


void Processor::cmdPtr2Right()
{
	if (m_ptr < RAM_SIZE - 1)
	{
		++m_ptr;
	}
	else
	{
		m_ptr = 0;
	}
}


void Processor::cmdPtr2Left()
{
	if (m_ptr > 0)
	{
		--m_ptr;
	}
	else
	{
		m_ptr = RAM_SIZE - 1;
	}
}


void Processor::cmdInc()
{
	if (++m_ram[m_ptr] > MAX_DATA)
	{
		m_ram[m_ptr] = MIN_DATA;
	}
}


void Processor::cmdDec()
{
	if (--m_ram[m_ptr] < MIN_DATA)
	{
		m_ram[m_ptr] = MAX_DATA;
	}
}

//#################################################################################################

void Processor::jobReadNear()
{
	int total = 0;

	auto& nearBlocks = m_pBlock->getLinkerPort().getTargetList();

	for (auto* pNear : nearBlocks)
	{
		total += pNear->getData();
	}

	m_ram[m_ptr] = total;
}

