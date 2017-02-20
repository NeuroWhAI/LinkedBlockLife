#ifndef __TILE_H__
#define __TILE_H__


#include <vector>

#include "CodeAdapter\EasyCA.h"




class Block;


class Tile
{
public:
	Tile();


private:
	std::vector<Block*>* m_pBlocks;
	std::vector<Block*>* m_pNextBlocks;
	std::vector<Block*> m_blocks1, m_blocks2;


private:
	caDraw::VectorF m_wind;
	caDraw::VectorF m_nextWind;
	float m_pressure;
	float m_nextPressure;


public:
	bool isBlocked() const;
	const std::vector<Block*>& getBlocks() const;
	void addBlock(Block* pBlock);
	const std::vector<Block*>& getNextBlocks() const;
	void addNextBlock(Block* pBlock);


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
	void updateBlockList();
	void updateAir();
};


#endif