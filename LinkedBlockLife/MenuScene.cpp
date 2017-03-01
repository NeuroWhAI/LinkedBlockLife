#include "MenuScene.h"

#include "MainScene.h"
#include "LabScene.h"




MenuScene::MenuScene()
{

}


MenuScene::~MenuScene()
{

}

//###########################################################################

void MenuScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();


	m_font = m_pool.createFont(L"DefaultFont");
	m_font->loadFromFile("NanumGothic.ttf");
	m_font->setCharacterSize(18);


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };

	m_toMain = canew<caUI::Button>();
	m_toMain->setFont(m_font);
	m_toMain->setSize({ 256, 48 });
	m_toMain->transform.position = { winSize.width / 2.0f - 128.0f, 256.0f };
	m_toMain->setText("Main simulation");
	m_toMain->WhenClick = [this](const caUI::TouchEventArgs& args)
	{
		this->reserveNextScene<MainScene>();
	};

	m_toLab = canew<caUI::Button>();
	m_toLab->setFont(m_font);
	m_toLab->setSize({ 256, 48 });
	m_toLab->transform.position = { winSize.width / 2.0f - 128.0f, 256.0f + 64.0f };
	m_toLab->setText("Lab");
	m_toLab->WhenClick = [this](const caUI::TouchEventArgs& args)
	{
		this->reserveNextScene<LabScene>();
	};

	m_exit = canew<caUI::Button>();
	m_exit->setFont(m_font);
	m_exit->setSize({ 256, 48 });
	m_exit->transform.position = { winSize.width / 2.0f - 128.0f, 256.0f + 64.0f * 2 };
	m_exit->setText("Exit");
	m_exit->WhenClick = [this](const caUI::TouchEventArgs& args)
	{
		this->reserveNextScene(nullptr);
	};

	m_panel->addControl(m_toMain);
	m_panel->addControl(m_toLab);
	m_panel->addControl(m_exit);
	

	addPanel(m_panel);
}


void MenuScene::onRelease()
{

}


void MenuScene::onUpdate(caDraw::Window& owner)
{
	
}


void MenuScene::onDrawBack(caDraw::Graphics& g)
{
	
}


void MenuScene::onDrawFront(caDraw::Graphics& g)
{

}

