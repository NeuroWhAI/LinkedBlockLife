#ifndef __JOB_SOLVER_H__
#define __JOB_SOLVER_H__


#include <vector>
#include <deque>

#include "JobArgument.h"




class Processor;


class JobSolver
{
private:
	template <typename T>
	using JobBoard = std::vector<std::deque<T>>;


public:
	explicit JobSolver(std::size_t coreCount);


private:
	std::size_t m_currentCore;


private:
	JobBoard<JobWriteBlockData> m_jobsWriteBlockData;


public:
	void updateProcessor(std::size_t coreIndex, Processor& proc);
	void performAllJobs();


public:
	void jobWriteBlockData(const JobWriteBlockData& args);


private:
	void clearAllJobs();
	void writeBlockData();
};


#endif