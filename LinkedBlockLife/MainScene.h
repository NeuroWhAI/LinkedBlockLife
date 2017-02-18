#ifndef __HELLO_SCENE_H__
#define __HELLO_SCENE_H__


#include <chrono>

#include "CodeAdapter\EasyCA.h"

#include "DrawableWorld.h"




class MainScene : public caUtil::Scene
{
public:
	MainScene();
	virtual ~MainScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;


private:
	caDraw::PanelPtr m_panel;


private:
	std::chrono::duration<double> m_updateDelay;


private:
	DrawableWorld m_world;
	caDraw::Transform m_worldTransform;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;
};


#endif