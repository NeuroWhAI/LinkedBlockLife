#ifndef __LINKER_H__
#define __LINKER_H__


#include "CodeAdapter\EasyCA.h"




class Block;


class Linker
{
public:
	Linker(Block& first, Block& second);


private:
	const float m_defaultLength;
	caDraw::VectorF m_elasticity;


private:
	Block& m_first;
	Block& m_second;


public:
	Block& getFirst();
	const Block& getFirst() const;
	Block& getSecond();
	const Block& getSecond() const;


public:
	void calculateElasticity();
	const caDraw::VectorF& getElasticity() const;
};


#endif