#ifndef __TILE_H__
#define __TILE_H__


#include "CodeAdapter\EasyCA.h"




class Block;


class Tile
{
public:
	Tile();


private:
	Block* m_pBlock;


private:
	caDraw::VectorF m_wind;
	caDraw::VectorF m_nextWind;
	float m_pressure;
	float m_nextPressure;


public:
	bool isBlocked() const;
	Block* getBlock() const;
	void setBlock(Block* pBlock);


public:
	const caDraw::VectorF& getWind() const;
	const caDraw::VectorF& getNextWind() const;
	void setNextWind(const caDraw::VectorF& wind);
	void addNextWind(const caDraw::VectorF& wind);

	float getPressure() const;
	float getNextPressure() const;
	void setNextPressure(float pressure);
	void addNextPressure(float pressure);


public:
	void updateAir();
};


#endif