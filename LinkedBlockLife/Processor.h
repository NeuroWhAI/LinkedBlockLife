#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__


#include <vector>

#include "CodeAdapter\EasyCA.h"




class Block;


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


public:
	bool willDisappear() const;
	void setBlock(Block* pBlock);
	Block* getBlock();


public:
	void execute();


private:
	void moveToNextBlock();


private:
	void cmdDoJob();
	void cmdPtr2Right();
	void cmdPtr2Left();
	void cmdInc();
	void cmdDec();


private:
	void jobReadNear();
};


#endif