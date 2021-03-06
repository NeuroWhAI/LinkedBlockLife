#ifndef __BLOCK_H__
#define __BLOCK_H__


#include "CodeAdapter\EasyCA.h"

#include "LinkerPort.h"




class Linker;
class Tile;


class Block
{
public:
	static const int DEFAULT_ENERGY = 100;


public:
	Block();


private:
	caDraw::VectorF m_position;
	caDraw::VectorF m_speed;
	caDraw::VectorF m_force;
	int m_energy;


private:
	LinkerPort m_linkerPort;


private:
	int m_data;


public:
	const caDraw::VectorF& getPosition() const;
	void setPosition(const caDraw::VectorF& position);
	void addPosition(const caDraw::VectorF& position);

	const caDraw::VectorF& getSpeed() const;
	void setSpeed(const caDraw::VectorF& speed);

	const caDraw::VectorF& getForce() const;
	void setForce(const caDraw::VectorF& force);
	void addForce(const caDraw::VectorF& force);

	int getEnergy() const;
	void setEnergy(int energy);
	void addEnergy(int energy);


public:
	bool willDisappear() const;
	void updateSpeed();
	void updatePosition();


public:
	LinkerPort& getLinkerPort();


public:
	int getData() const;
	void setData(int data);
};


#endif