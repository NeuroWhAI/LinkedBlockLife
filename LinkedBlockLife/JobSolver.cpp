#include "JobSolver.h"

#include "Processor.h"
#include "Block.h"




JobSolver::JobSolver(std::size_t coreCount)
	: m_currentCore(0)

	, m_jobsWriteBlockData(coreCount)
{

}

//#################################################################################################

void JobSolver::updateProcessor(std::size_t coreIndex, Processor& proc)
{
	m_currentCore = coreIndex;

	proc.execute(*this);
}


void JobSolver::performAllJobs()
{
	// 작업 수행
	writeBlockData();


	clearAllJobs();
}

//#################################################################################################

void JobSolver::jobWriteBlockData(const JobWriteBlockData& args)
{
	m_jobsWriteBlockData[m_currentCore].emplace_back(args);
}

//#################################################################################################

void JobSolver::clearAllJobs()
{
	for (auto& jobs : m_jobsWriteBlockData)
	{
		jobs.clear();
	}
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

