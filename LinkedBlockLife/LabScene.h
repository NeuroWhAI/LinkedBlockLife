#ifndef __LAB_SCENE_H__
#define __LAB_SCENE_H__


#include <chrono>

#include "CodeAdapter\EasyCA.h"




class LabScene : public caUtil::Scene
{
public:
	LabScene();
	virtual ~LabScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;


private:
	caDraw::PanelPtr m_panel;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;
};


#endif