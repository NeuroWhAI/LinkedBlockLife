#ifndef __BLOCK_H__
#define __BLOCK_H__


#include "CodeAdapter\EasyCA.h"

#include "LinkerPort.h"




class Linker;


class Block
{
public:
	Block();


private:
	caDraw::VectorF m_position;
	caDraw::VectorF m_speed;


private:
	LinkerPort m_linkerPort;


public:
	const caDraw::VectorF& getPosition() const;
	void setPosition(const caDraw::VectorF& position);
	void addPosition(const caDraw::VectorF& position);

	const caDraw::VectorF& getSpeed() const;
	void setSpeed(const caDraw::VectorF& speed);
	void addSpeed(const caDraw::VectorF& speed);


public:
	void updatePosition();


public:
	LinkerPort& getLinkerPort();
};


#endif