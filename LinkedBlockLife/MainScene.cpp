#include "MainScene.h"

#include <sstream>
#include <thread>

#include "MenuScene.h"




MainScene::MainScene()
	: m_world(8 * std::thread::hardware_concurrency())
	, m_worldTransform(caDraw::Transform::Identity)
{
	
}


MainScene::~MainScene()
{
	
}

//###########################################################################

void MainScene::onInitialize(caDraw::Window& owner)
{
	auto winSize = owner.getSize();
	
	
	m_font = m_pool.createFont(L"DefaultFont");
	m_font->loadFromFile("NanumGothic.ttf");
	m_font->setCharacterSize(18);


	m_panel = caFactory->createPanel();
	m_panel->size = static_cast<caDraw::SizeF>(winSize);
	m_panel->transform.position = { 0, 0 };

	m_toggleDraw = canew<caUI::CheckBox>();
	m_toggleDraw->setFont(m_font);
	m_toggleDraw->check(true);
	m_toggleDraw->transform.position = { 2.0f, winSize.height - 32.0f - 2.0f };
	m_toggleDraw->setSize({ 150, 32 });
	m_toggleDraw->setText("Draw sim");
	m_toggleDraw->setTextMargin({ 2, 0 });
	m_toggleDraw->setBackColor(caDraw::Color::Transparent);

	m_panel->addControl(m_toggleDraw);


	addPanel(m_panel);
}


void MainScene::onRelease()
{
	
}


void MainScene::onUpdate(caDraw::Window& owner)
{
	using std::chrono::high_resolution_clock;


	auto beginTime = high_resolution_clock::now();


	// Go to menu when press Esc.
	if (caKeyboard->isKeyDown(caSys::Keys::Escape))
	{
		reserveNextScene<MenuScene>();
	}


	// Update Sim.
	m_world.update();


	auto endTime = high_resolution_clock::now();
	m_updateDelay = endTime - beginTime;
}


void MainScene::onDrawBack(caDraw::Graphics& g)
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;


	auto& textArt = g.textArtist;


	auto beginTime = high_resolution_clock::now();


	if (m_toggleDraw->isChecked())
	{
		// Render Sim.
		m_world.draw(g, m_worldTransform);
	}


	auto endTime = high_resolution_clock::now();
	duration<double> drawDelay = endTime - beginTime;


	// Draw Delay
	std::ostringstream oss;
	oss << (1.0 / m_updateDelay.count()) << "ups" << std::endl;
	oss << (1.0 / drawDelay.count()) << "rps";

	textArt->beginDrawString(*m_font);
	textArt->drawString(oss.str(), 2, 2, caDraw::Color::Red);
	textArt->endDrawString();
}


void MainScene::onDrawFront(caDraw::Graphics& g)
{

}

