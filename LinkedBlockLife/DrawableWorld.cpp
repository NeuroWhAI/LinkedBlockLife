#include "DrawableWorld.h"




DrawableWorld::DrawableWorld(std::size_t size)
	: World(size)

	, m_pixelPerUnit(4)
{

}

//#################################################################################################

void DrawableWorld::onDraw(Graphics& g, const Transform& parentTransform)
{
	drawAir(g);
}

//#################################################################################################

void DrawableWorld::drawAir(Graphics& g)
{
	const std::size_t boardSize = m_board.size();


	const float pixelPerUnitF = static_cast<float>(m_pixelPerUnit);

	auto& rectArt = g.rectangleArtist;
	auto& circleArt = g.ellipseArtist;
	auto& lineArt = g.lineArtist;


	lineArt->beginDrawLine(1.5f);


	for (std::size_t y = 0; y < boardSize; ++y)
	{
		for (std::size_t x = 0; x < boardSize; ++x)
		{
			const auto& vec = m_board[y][x]->getWind();

			float p = m_board[y][x]->getPressure();
			float v = vec.getLength();

			caDraw::Color color{
				static_cast<i32>(std::min(v, 255.0f)),
				static_cast<i32>((p < 0) ? 0 : std::min(p, 255.0f)),
				static_cast<i32>((p > 0) ? 0 : std::min(-p, 255.0f)),
			};

			lineArt->drawLine(256 + (int)x * 8, 256 + (int)y * 8,
				256 + x * 8 + (int)(vec.x), 256 + y * 8 + (int)(vec.y),
				color);
		}
	}


	lineArt->endDrawLine();


	rectArt->beginFillRectangle();


	for (std::size_t y = 0; y < boardSize; ++y)
	{
		for (std::size_t  x = 0; x < boardSize; ++x)
		{
			float p = m_board[y][x]->getPressure();
			float v = m_board[y][x]->getWind().getLength();

			caDraw::Color color{
				static_cast<i32>(std::min(v, 255.0f)),
				static_cast<i32>((p < 0) ? 0 : std::min(p, 255.0f)),
				static_cast<i32>((p > 0) ? 0 : std::min(-p, 255.0f)),
			};

			if (m_board[y][x]->isBlocked())
			{
				color = caDraw::Color::Gray;
			}

			rectArt->fillRectangle(x * m_pixelPerUnit, y * m_pixelPerUnit, 4, 4, color);
		}
	}


	rectArt->endFillRectangle();


	circleArt->beginFillEllipse();


	for (auto& block : m_blocks)
	{
		circleArt->fillEllipse(block->getPosition().x * m_pixelPerUnit,
			block->getPosition().y * m_pixelPerUnit,
			pixelPerUnitF, pixelPerUnitF,
			caDraw::Color::Gray);
	}


	circleArt->endFillEllipse();
}

