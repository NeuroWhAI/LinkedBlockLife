#ifndef __LAB_SCENE_H__
#define __LAB_SCENE_H__


#include <chrono>

#include "CodeAdapter\EasyCA.h"

#include "LabWorld.h"




class LabScene : public caUtil::Scene
{
private:
	using Graphics = caDraw::Graphics;


public:
	LabScene();
	virtual ~LabScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;


private:
	caDraw::PanelPtr m_panel;
	caUI::CheckBoxPtr m_toggleUpdate;
	caUI::CheckBoxPtr m_toggleGrid;


private:
	LabWorld m_world;
	caDraw::Transform m_worldTransform;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;


private:
	void moveWorld();


private:
	void drawGrid(Graphics& g);
};


#endif