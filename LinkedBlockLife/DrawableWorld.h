#ifndef __DRAWABLE_WORLD_H__
#define __DRAWABLE_WORLD_H__


#include "World.h"

#include "CodeAdapter\EasyCA.h"




class DrawableWorld : public World, public caDraw::Drawable
{
private:
	using Graphics = caDraw::Graphics;
	using Transform = caDraw::Transform;


public:
	explicit DrawableWorld(std::size_t size);


protected:
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;


private:
	void drawAir(Graphics& g);
};


#endif