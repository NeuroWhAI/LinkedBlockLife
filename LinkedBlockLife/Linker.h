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
	caDraw::VectorF m_dir;
	float m_length;


private:
	Block& m_first;
	Block& m_second;


public:
	Block& getFirst();
	Block& getSecond();


public:
	void calculateDir();
	caDraw::VectorF calculateElasticity() const;
};


#endif