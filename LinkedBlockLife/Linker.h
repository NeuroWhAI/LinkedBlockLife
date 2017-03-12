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
	* �� �޼ҵ带 ȣ���ߴٴ� ���� ��Ŀ�� ���� ������ �����ٴ� �ǹ��̰�
	* �� Solver�� ���� ��Ŀ�� ���Ÿ� �߱���.
	*/
	void setDisconnectionFlag();
	bool isDisconnected() const;
};


#endif