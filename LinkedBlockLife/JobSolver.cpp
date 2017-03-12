#include "JobSolver.h"

#include "Processor.h"
#include "Block.h"
#include "Linker.h"




JobSolver::JobSolver(std::size_t coreCount)
	: m_jobsWriteBlockData(coreCount)
	, m_jobsBoomLinker(coreCount)
{

}

//#################################################################################################

void JobSolver::updateProcessor(std::size_t coreIndex, Processor& proc)
{
	proc.execute(coreIndex, *this);
}


void JobSolver::performAllJobs()
{
	// 작업 수행
	writeBlockData();
	boomLinker();


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

//#################################################################################################

void JobSolver::clearAllJobs()
{
	for (auto& jobs : m_jobsWriteBlockData)
		jobs.clear();

	for (auto& jobs : m_jobsBoomLinker)
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

