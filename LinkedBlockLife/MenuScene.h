#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__


#include <chrono>

#include "CodeAdapter\EasyCA.h"




class MenuScene : public caUtil::Scene
{
public:
	MenuScene();
	virtual ~MenuScene();


private:
	caUtil::ResourcePool m_pool;
	caDraw::FontPtr m_font;


private:
	caDraw::PanelPtr m_panel;
	caUI::ButtonPtr m_toMain;
	caUI::ButtonPtr m_toLab;
	caUI::ButtonPtr m_exit;


public:
	virtual void onInitialize(caDraw::Window& owner) override;
	virtual void onRelease() override;
	virtual void onUpdate(caDraw::Window& owner) override;
	virtual void onDrawBack(caDraw::Graphics& g) override;
	virtual void onDrawFront(caDraw::Graphics& g) override;
};


#endif