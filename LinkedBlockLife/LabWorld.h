#ifndef __LAB_WORLD_H__
#define __LAB_WORLD_H__


#include "DrawableWorld.h"




class LabWorld : public DrawableWorld
{
public:
	explicit LabWorld(std::size_t size);
	virtual ~LabWorld() = default;


public:
	void updateLab(const caDraw::Transform& transform);
};


#endif