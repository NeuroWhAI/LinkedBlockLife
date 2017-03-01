#include "LabScene.h"

#include "MenuScene.h"




LabScene::LabScene()
{

}


LabScene::~LabScene()
{

}

//###########################################################################

void LabScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();


	m_font = m_pool.createFont(L"DefaultFont");
	m_font->loadFromFile("NanumGothic.ttf");
	m_font->setCharacterSize(18);


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };


	addPanel(m_panel);
}


void LabScene::onRelease()
{

}


void LabScene::onUpdate(caDraw::Window& owner)
{
	// Go to menu when press Esc.
	if (caKeyboard->isKeyDown(caSys::Keys::Escape))
	{
		reserveNextScene<MenuScene>();
	}
}


void LabScene::onDrawBack(caDraw::Graphics& g)
{

}


void LabScene::onDrawFront(caDraw::Graphics& g)
{

}

