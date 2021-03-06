#include "Processor.h"

#include <cassert>
#include <numeric>

#include "Block.h"
#include "JobSolver.h"
#include "WorldInteractor.h"




Processor::Processor(Block* pBlock, const caDraw::VectorF& dir)
	: m_willDie(false)
	, m_pPrevBlock(nullptr)
	, m_pBlock(pBlock)
	, m_dir(dir)
	, m_pNextBlock(pBlock)
	, m_nextDir(dir)

	, m_ram(RAM_SIZE, 0)
	, m_ptr(0)
	, m_register(0)
	, m_condition(false)

	, m_pJobSolver(nullptr)
	, m_tempCoreIndex(0)
	, m_pWorld(nullptr)
{
	static_assert(MAX_DATA > MIN_DATA, "Must MAX_DATA > MIN_DATA");


	m_cmdList.emplace_back(nullptr);
	m_cmdList.emplace_back(&Processor::cmdZero);
	m_cmdList.emplace_back(&Processor::cmdDoJob);
	m_cmdList.emplace_back(&Processor::cmdDoJobConditionally);
	m_cmdList.emplace_back(&Processor::cmdPtr2Left);
	m_cmdList.emplace_back(&Processor::cmdPtr2Right);
	m_cmdList.emplace_back(&Processor::cmdInc);
	m_cmdList.emplace_back(&Processor::cmdDec);
	m_cmdList.emplace_back(&Processor::cmdReadReg);
	m_cmdList.emplace_back(&Processor::cmdWriteReg);
	m_cmdList.emplace_back(&Processor::cmdZeroPtr);
	m_cmdList.emplace_back(&Processor::cmdSum);
	m_cmdList.emplace_back(&Processor::cmdMul);
	m_cmdList.emplace_back(&Processor::cmdEqual);
	m_cmdList.emplace_back(&Processor::cmdBig);
	m_cmdList.emplace_back(&Processor::cmdSmall);
	m_cmdList.emplace_back(&Processor::cmdReadEnergy);

	m_jobList.emplace_back(nullptr);
	m_jobList.emplace_back(&Processor::jobAccumulateNear);
	m_jobList.emplace_back(&Processor::jobWriteData);
	m_jobList.emplace_back(&Processor::jobBoomLinker);
	m_jobList.emplace_back(&Processor::jobSpreadEnergy);
	m_jobList.emplace_back(&Processor::jobTakeEnergy);
	m_jobList.emplace_back(&Processor::jobConnectLinker);
	m_jobList.emplace_back(&Processor::jobGenerateProcessor);
	m_jobList.emplace_back(&Processor::jobGenerateBlock);
	m_jobList.emplace_back(&Processor::jobInverseDirection);
	m_jobList.emplace_back(&Processor::jobTurnDirection);
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

void Processor::execute(std::size_t coreIndex, JobSolver& jobSolver, WorldInteractor& interactor)
{
	assert(m_pBlock != nullptr);


	m_pJobSolver = &jobSolver;
	m_tempCoreIndex = coreIndex;
	m_pWorld = &interactor;


	calculateNextBlock();

	// 다음에 위치할 블럭이 사라질 예정이거나
	// 방금 거쳤던 블럭 말고는 갈 곳이 없으면
	// 프로세서는 죽는다.
	m_willDie = (m_pNextBlock->willDisappear() || m_pNextBlock == m_pBlock);


	// 현재 블럭의 명령어 실행.
	auto cmd = m_pBlock->getData();

	if (cmd > 0 && static_cast<std::size_t>(cmd) < m_cmdList.size())
	{
		(this->*m_cmdList[cmd])();
	}


	m_pPrevBlock = m_pBlock;
	m_pBlock = m_pNextBlock;
	m_dir = m_nextDir;
}

//#################################################################################################

void Processor::calculateNextBlock()
{
	auto& centerPos = m_pBlock->getPosition();

	float maxScore = std::numeric_limits<float>::lowest();

	auto& nearBlocks = m_pBlock->getLinkerPort().getTargetList();

	for (auto* const pNearBlock : nearBlocks)
	{
		// 이전에 왔던 블럭은 제외.
		if (pNearBlock == m_pPrevBlock)
			continue;

		auto subVec = pNearBlock->getPosition() - centerPos;

		float score = m_dir.dotProduct(subVec);

		if (score > maxScore)
		{
			maxScore = score;
			m_pNextBlock = pNearBlock;
			m_nextDir = subVec;
		}
	}
}


void Processor::setRamAt(std::size_t index, int data)
{
	if (data > MAX_DATA)
		data = MAX_DATA;
	else if (data < MIN_DATA)
		data = MIN_DATA;

	m_ram[index] = data;
}

//#################################################################################################

void Processor::cmdZero()
{
	setRamAt(m_ptr, 0);
}


void Processor::cmdDoJob()
{
	assert(m_pJobSolver != nullptr);


	// 현재 포인터가 위치한 셀의 값에 해당하는 작업 수행.
	auto job = m_ram[m_ptr];

	if (job > 0 && static_cast<std::size_t>(job) < m_jobList.size())
	{
		(this->*m_jobList[job])();
	}
}


void Processor::cmdDoJobConditionally()
{
	if (m_condition)
	{
		cmdDoJob();
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
	setRamAt(m_ptr, m_ram[m_ptr] + 1);
}


void Processor::cmdDec()
{
	setRamAt(m_ptr, m_ram[m_ptr] - 1);
}


void Processor::cmdReadReg()
{
	setRamAt(m_ptr, m_register);
}


void Processor::cmdWriteReg()
{
	m_register = m_ram[m_ptr];
}


void Processor::cmdZeroPtr()
{
	m_ptr = 0;
}


void Processor::cmdSum()
{
	setRamAt(m_ptr, m_ram[m_ptr] + m_register);
}


void Processor::cmdMul()
{
	setRamAt(m_ptr, m_ram[m_ptr] * m_register);
}


void Processor::cmdEqual()
{
	m_condition = (m_ram[m_ptr] == m_register);
}


void Processor::cmdBig()
{
	m_condition = (m_ram[m_ptr] > m_register);
}


void Processor::cmdSmall()
{
	m_condition = (m_ram[m_ptr] < m_register);
}


void Processor::cmdReadEnergy()
{
	setRamAt(m_ptr, m_pBlock->getEnergy());
}

//#################################################################################################

void Processor::jobAccumulateNear()
{
	int total = 0;

	auto& nearBlocks = m_pBlock->getLinkerPort().getTargetList();

	for (auto* pNear : nearBlocks)
	{
		total += pNear->getData();
	}

	setRamAt(m_ptr, total);
}


void Processor::jobWriteData()
{
	m_pJobSolver->jobWriteBlockData(m_tempCoreIndex, { m_pBlock, m_register });
}


void Processor::jobBoomLinker()
{
	auto& linkers = m_pBlock->getLinkerPort().getLinkerList();

	for (auto* pLinker : linkers)
	{
		m_pJobSolver->jobBoomLinker(m_tempCoreIndex, { pLinker });
	}
}


void Processor::jobSpreadEnergy()
{
	int leftEnergy = m_pBlock->getEnergy();

	if (leftEnergy > 0)
	{
		auto& nearBlocks = m_pBlock->getLinkerPort().getTargetList();

		for (auto* pNear : nearBlocks)
		{
			m_pJobSolver->jobGiveEnergy(m_tempCoreIndex, { m_pBlock, pNear, 1 });

			--leftEnergy;
			if (leftEnergy <= 0)
				break;
		}
	}
}


void Processor::jobTakeEnergy()
{
	auto& nearBlocks = m_pBlock->getLinkerPort().getTargetList();

	for (auto* pNear : nearBlocks)
	{
		if (pNear->getEnergy() >= 1)
		{
			m_pJobSolver->jobGiveEnergy(m_tempCoreIndex, { pNear, m_pBlock, 1 });
		}
	}
}


void Processor::jobConnectLinker()
{
	m_pJobSolver->jobConnectLinker(m_tempCoreIndex, { m_pBlock });
}


void Processor::jobGenerateProcessor()
{
	m_pJobSolver->jobGenerateProcessor(m_tempCoreIndex, { m_pBlock, m_dir });
}


void Processor::jobGenerateBlock()
{
	if (m_pBlock->willDisappear() == false
		&& m_pNextBlock->willDisappear() == false
		&& m_pBlock->getEnergy() > Block::DEFAULT_ENERGY)
	{
		auto position = (m_pBlock->getPosition() + m_pNextBlock->getPosition()) / 2.0f;

		m_pJobSolver->jobGenerateBlock(m_tempCoreIndex, { m_pBlock, m_pNextBlock, position, m_register });
	}
}


void Processor::jobInverseDirection()
{
	m_dir = -m_dir;
}


void Processor::jobTurnDirection()
{
	if (m_register > 0)
	{
		m_dir = { -m_dir.y, m_dir.x };
	}
	else if (m_register < 0)
	{
		m_dir = { m_dir.y, -m_dir.x };
	}
}

