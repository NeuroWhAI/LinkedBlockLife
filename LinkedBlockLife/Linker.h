#ifndef __LINKER_H__
#define __LINKER_H__


#include "CodeAdapter\EasyCA.h"




class Block;


class Linker
{
private:
	static const float DEFAULT_LENGTH;


public:
	Linker(Block& first, Block& second);


private:
	Block& m_first;
	Block& m_second;


private:
	float m_targetLength;
	caDraw::VectorF m_elasticity;


private:
	bool m_disconnected;


public:
	Block& getFirst();
	const Block& getFirst() const;
	Block& getSecond();
	const Block& getSecond() const;


public:
	void calculateElasticity();
	const caDraw::VectorF& getElasticity() const;
	void boom(float scale);


public:
	/*
	* 이 메소드를 호출했다는 것은 링커와 블럭의 연결을 끊었다는 의미이고
	* 곧 Solver에 의한 링커의 제거를 야기함.
	*/
	void setDisconnectionFlag();
	bool isDisconnected() const;
};


#endif