#ifndef __JOB_ARGUMENT_H__
#define __JOB_ARGUMENT_H__


#include "CodeAdapter\EasyCA.h"




class Block;
class Linker;


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


class JobBoomLinker
{
public:
	JobBoomLinker(Linker* pTarget)
		: pTarget(pTarget)
	{ }

public:
	Linker* pTarget;
};


class JobGiveEnergy
{
public:
	JobGiveEnergy(Block* pSrc, Block* pDest, int energy)
		: pSrc(pSrc)
		, pDest(pDest)
		, energy(energy)
	{ }

public:
	Block* pSrc;
	Block* pDest;
	int energy;
};


class JobConnectLinker
{
public:
	JobConnectLinker(Block* pCenter)
		: pCenter(pCenter)
	{ }

public:
	Block* pCenter;
};


class JobGenerateProcessor
{
public:
	JobGenerateProcessor(Block* pBlock, const caDraw::VectorF& dir)
		: pBlock(pBlock)
		, dir(dir)
	{ }

public:
	Block* pBlock;
	caDraw::VectorF dir;
};


class JobGenerateBlock
{
public:
	JobGenerateBlock(Block* pParent, int data, const caDraw::VectorF& dir)
		: pParent(pParent)
		, data(data)
		, dir(dir)
	{ }

public:
	Block* pParent;
	int data;
	caDraw::VectorF dir;
};


#endif