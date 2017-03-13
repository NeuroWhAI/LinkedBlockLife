#ifndef __JOB_SOLVER_H__
#define __JOB_SOLVER_H__


#include <vector>
#include <memory>

#include "JobArgument.h"




class WorldInteractor;
class Processor;
class Block;
class Linker;
class Tile;
class LinkerPort;


class JobSolver
{
private:
	template <typename T>
	using JobBoard = std::vector<std::vector<T>>;
	using TileBoard = std::vector<std::vector<std::unique_ptr<Tile>>>;


public:
	explicit JobSolver(std::size_t coreCount);


private:
	JobBoard<JobWriteBlockData> m_jobsWriteBlockData;
	JobBoard<JobBoomLinker> m_jobsBoomLinker;
	JobBoard<JobGiveEnergy> m_jobsGiveEnergy;
	JobBoard<JobConnectLinker> m_jobsConnectLinker;


public:
	void updateProcessor(std::size_t coreIndex, Processor& proc);
	void performAllJobs(WorldInteractor& interactor);


public:
	void jobWriteBlockData(std::size_t coreIndex, const JobWriteBlockData& args);
	void jobBoomLinker(std::size_t coreIndex, const JobBoomLinker& args);
	void jobGiveEnergy(std::size_t coreIndex, const JobGiveEnergy& args);
	void jobConnectLinker(std::size_t coreIndex, const JobConnectLinker& args);


private:
	void clearAllJobs();
	void writeBlockData();
	void boomLinker();
	void giveEnergy();
	void connectLinker(WorldInteractor& interactor);
};


#endif