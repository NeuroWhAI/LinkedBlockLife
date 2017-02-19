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
	caDraw::VectorF m_force;


private:
	LinkerPort m_linkerPort;


public:
	const caDraw::VectorF& getPosition() const;
	void setPosition(const caDraw::VectorF& position);
	void addPosition(const caDraw::VectorF& position);

	const caDraw::VectorF& getSpeed() const;
	void setSpeed(const caDraw::VectorF& speed);

	const caDraw::VectorF& getForce() const;
	void setForce(const caDraw::VectorF& force);
	void addForce(const caDraw::VectorF& force);


public:
	void update();


public:
	LinkerPort& getLinkerPort();
};


#endif