#include "MainScene.h"

#include <sstream>




MainScene::MainScene()
	: m_world(64)
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


	addPanel(m_panel);
}


void MainScene::onRelease()
{
	
}


void MainScene::onUpdate(caDraw::Window& owner)
{
	using std::chrono::high_resolution_clock;


	auto beginTime = high_resolution_clock::now();


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


	// Render Sim.
	m_world.draw(g, m_worldTransform);


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

