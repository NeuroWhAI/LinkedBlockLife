#ifndef __JOB_SOLVER_H__
#define __JOB_SOLVER_H__


#include <vector>

#include "JobArgument.h"




class Processor;


class JobSolver
{
private:
	template <typename T>
	using JobBoard = std::vector<std::vector<T>>;


public:
	explicit JobSolver(std::size_t coreCount);


private:
	JobBoard<JobWriteBlockData> m_jobsWriteBlockData;
	JobBoard<JobBoomLinker> m_jobsBoomLinker;
	JobBoard<JobGiveEnergy> m_jobsGiveEnergy;


public:
	void updateProcessor(std::size_t coreIndex, Processor& proc);
	void performAllJobs();


public:
	void jobWriteBlockData(std::size_t coreIndex, const JobWriteBlockData& args);
	void jobBoomLinker(std::size_t coreIndex, const JobBoomLinker& args);
	void jobGiveEnergy(std::size_t coreIndex, const JobGiveEnergy& args);


private:
	void clearAllJobs();
	void writeBlockData();
	void boomLinker();
	void giveEnergy();
};


#endif