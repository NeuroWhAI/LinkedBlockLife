#include "JobSolver.h"

#include "WorldInteractor.h"
#include "Processor.h"
#include "Block.h"
#include "Linker.h"
#include "Tile.h"




JobSolver::JobSolver(std::size_t coreCount)
	: m_jobsWriteBlockData(coreCount)
	, m_jobsBoomLinker(coreCount)
	, m_jobsGiveEnergy(coreCount)
	, m_jobsConnectLinker(coreCount)
	, m_jobsGenerateProcessor(coreCount)
{

}

//#################################################################################################

void JobSolver::updateProcessor(std::size_t coreIndex, Processor& proc)
{
	proc.execute(coreIndex, *this);
}


void JobSolver::performAllJobs(WorldInteractor& interactor)
{
	// 작업 수행
	writeBlockData();
	boomLinker();
	giveEnergy();
	connectLinker(interactor);
	generateProcessor(interactor);


	clearAllJobs();
}

//#################################################################################################

void JobSolver::jobWriteBlockData(std::size_t coreIndex, const JobWriteBlockData& args)
{
	m_jobsWriteBlockData[coreIndex].emplace_back(args);
}


void JobSolver::jobBoomLinker(std::size_t coreIndex, const JobBoomLinker& args)
{
	m_jobsBoomLinker[coreIndex].emplace_back(args);
}


void JobSolver::jobGiveEnergy(std::size_t coreIndex, const JobGiveEnergy& args)
{
	m_jobsGiveEnergy[coreIndex].emplace_back(args);
}


void JobSolver::jobConnectLinker(std::size_t coreIndex, const JobConnectLinker& args)
{
	m_jobsConnectLinker[coreIndex].emplace_back(args);
}


void JobSolver::jobGenerateProcessor(std::size_t coreIndex, const JobGenerateProcessor& args)
{
	m_jobsGenerateProcessor[coreIndex].emplace_back(args);
}

//#################################################################################################

void JobSolver::clearAllJobs()
{
	for (auto& jobs : m_jobsWriteBlockData)
		jobs.clear();

	for (auto& jobs : m_jobsBoomLinker)
		jobs.clear();

	for (auto& jobs : m_jobsGiveEnergy)
		jobs.clear();

	for (auto& jobs : m_jobsConnectLinker)
		jobs.clear();

	for (auto& jobs : m_jobsGenerateProcessor)
		jobs.clear();
}


void JobSolver::writeBlockData()
{
	for (auto& jobs : m_jobsWriteBlockData)
	{
		for (auto& arg : jobs)
		{
			arg.pTarget->setData(arg.data);
		}
	}
}


void JobSolver::boomLinker()
{
	for (auto& jobs : m_jobsBoomLinker)
	{
		for (auto& arg : jobs)
		{
			arg.pTarget->boom(32.0f);
		}
	}
}


void JobSolver::giveEnergy()
{
	for (auto& jobs : m_jobsGiveEnergy)
	{
		for (auto& arg : jobs)
		{
			if (arg.pSrc->getEnergy() >= arg.energy)
			{
				arg.pSrc->addEnergy(-arg.energy);
				arg.pDest->addEnergy(arg.energy);
			}
		}
	}
}


void JobSolver::connectLinker(WorldInteractor& interactor)
{
	for (auto& jobs : m_jobsConnectLinker)
	{
		for (auto& arg : jobs)
		{
			interactor.connectLinkerToNear(arg.pCenter);
		}
	}
}


void JobSolver::generateProcessor(WorldInteractor& interactor)
{
	for (auto& jobs : m_jobsGenerateProcessor)
	{
		for (auto& arg : jobs)
		{
			interactor.addProcessor(arg.pBlock, arg.dir);
		}
	}
}

