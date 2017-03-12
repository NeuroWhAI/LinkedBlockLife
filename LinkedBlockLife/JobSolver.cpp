#include "JobSolver.h"

#include "Processor.h"
#include "Block.h"




JobSolver::JobSolver(std::size_t coreCount)
	: m_jobsWriteBlockData(coreCount)
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


	clearAllJobs();
}

//#################################################################################################

void JobSolver::jobWriteBlockData(std::size_t coreIndex, const JobWriteBlockData& args)
{
	m_jobsWriteBlockData[coreIndex].emplace_back(args);
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

