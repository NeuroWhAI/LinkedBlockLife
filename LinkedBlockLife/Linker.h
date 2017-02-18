#ifndef __LINKER_H__
#define __LINKER_H__


#include "CodeAdapter\EasyCA.h"




class Block;


class Linker
{
public:
	Linker(Block& first, Block& second);


private:
	float m_defaultLength;


private:
	Block& m_first;
	Block& m_second;


public:
	Block& getFirst();
	Block& getSecond();


public:
	caDraw::VectorF calculateElasticity() const;
};


#endif