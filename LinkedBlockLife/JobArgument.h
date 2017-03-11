#ifndef __JOB_ARGUMENT_H__
#define __JOB_ARGUMENT_H__


class Block;


class JobWriteBlockData
{
public:
	JobWriteBlockData(Block* pTarget, int data)
		: pTarget(pTarget)
		, data(data)
	{ }

public:
	Block* pTarget;
	int data;
};


#endif