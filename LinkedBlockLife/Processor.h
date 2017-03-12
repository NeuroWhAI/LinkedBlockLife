#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__


#include <vector>

#include "CodeAdapter\EasyCA.h"




class Block;
class JobSolver;


class Processor
{
private:
	static const int MAX_DATA = 128;
	static const int MIN_DATA = -128;
	static const std::size_t RAM_SIZE = 64;


public:
	Processor(Block* pBlock, const caDraw::VectorF& dir);


private:
	bool m_willDie;
	Block* m_pBlock;
	caDraw::VectorF m_dir;


private:
	std::vector<int> m_ram;
	std::size_t m_ptr;
	std::vector<void(Processor::*)()> m_cmdList;


private:
	std::vector<void(Processor::*)()> m_jobList;
	JobSolver* m_pJobSolver;
	std::size_t m_tempCoreIndex;


public:
	bool willDisappear() const;
	void setBlock(Block* pBlock);
	Block* getBlock();


public:
	void execute(std::size_t coreIndex, JobSolver& jobSolver);


private:
	void moveToNextBlock();


private: // NOTE: 추가/삭제할 때마다 생성자 내용 갱신.
	void cmdDoJob();
	void cmdPtr2Right();
	void cmdPtr2Left();
	void cmdInc();
	void cmdDec();


private: // NOTE: 추가/삭제할 때마다 생성자 내용 갱신.
	void jobAccumulateNear();
	void jobWriteData();
};


#endif