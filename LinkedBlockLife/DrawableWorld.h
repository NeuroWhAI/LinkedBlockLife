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
	virtual ~DrawableWorld() = default;


private:
	int m_pixelPerUnit;


protected:
	virtual void onDraw(Graphics& g, const Transform& parentTransform) override;


public:
	int getPixelPerUnit() const;


private:
	void drawAir(Graphics& g);
	void drawBlock(Graphics& g);
	void drawProcessor(Graphics& g);
};


#endif