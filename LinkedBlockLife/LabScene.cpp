#include "LabScene.h"

#include "MenuScene.h"




LabScene::LabScene()
	: m_world(8 * std::thread::hardware_concurrency())
	, m_worldTransform(caDraw::Transform::Identity)
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

	m_toggleUpdate = canew<caUI::CheckBox>();
	m_toggleUpdate->setFont(m_font);
	m_toggleUpdate->check(false);
	m_toggleUpdate->transform.position = { 2.0f, winSize.height - 32.0f - 2.0f };
	m_toggleUpdate->setSize({ 150, 32 });
	m_toggleUpdate->setText("Update sim");
	m_toggleUpdate->setTextMargin({ 2, 0 });
	m_toggleUpdate->setBackColor(caDraw::Color::Transparent);

	m_toggleGrid = canew<caUI::CheckBox>();
	m_toggleGrid->setFont(m_font);
	m_toggleGrid->check(false);
	m_toggleGrid->transform.position = { 2.0f + 150.0f + 2.0f, winSize.height - 32.0f - 2.0f };
	m_toggleGrid->setSize({ 150, 32 });
	m_toggleGrid->setText("Draw grid");
	m_toggleGrid->setTextMargin({ 2, 0 });
	m_toggleGrid->setBackColor(caDraw::Color::Transparent);

	m_panel->addControl(m_toggleUpdate);
	m_panel->addControl(m_toggleGrid);


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


	moveWorld();


	if (m_toggleUpdate->isChecked())
	{
		m_world.update();
	}

	m_world.updateLab(m_worldTransform);
}


void LabScene::onDrawBack(caDraw::Graphics& g)
{
	m_world.draw(g, m_worldTransform);

	if (m_toggleGrid->isChecked())
	{
		drawGrid(g);
	}
}


void LabScene::onDrawFront(caDraw::Graphics& g)
{

}

//###########################################################################

void LabScene::moveWorld()
{
	const float pixel = static_cast<float>(m_world.getPixelPerUnit());

	if (caKeyboard->isKeyDown(caSys::Keys::Left))
	{
		m_worldTransform.position.move(pixel, 0);
	}
	else if (caKeyboard->isKeyDown(caSys::Keys::Right))
	{
		m_worldTransform.position.move(-pixel, 0);
	}

	if (caKeyboard->isKeyDown(caSys::Keys::Up))
	{
		m_worldTransform.position.move(0, pixel);
	}
	else if (caKeyboard->isKeyDown(caSys::Keys::Down))
	{
		m_worldTransform.position.move(0, -pixel);
	}
}

//###########################################################################

void LabScene::drawGrid(Graphics& g)
{
	auto& lineArt = g.lineArtist;


	lineArt->beginDrawLine(1.0f);


	const auto boardSize = m_world.getBoardSize();
	const float pixel = static_cast<float>(m_world.getPixelPerUnit());

	for (std::size_t i = 0; i <= boardSize; ++i)
	{
		const float beginPosition = static_cast<float>(i) * pixel;
		const float endPosition = static_cast<float>(boardSize) * pixel;

		lineArt->drawLine(beginPosition, 0.0f,
			beginPosition, endPosition,
			caDraw::Color::Gray);

		lineArt->drawLine(0.0f, beginPosition,
			endPosition, beginPosition,
			caDraw::Color::Gray);
	}


	lineArt->endDrawLine();
}

