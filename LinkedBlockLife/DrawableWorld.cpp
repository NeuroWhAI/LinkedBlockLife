#include "DrawableWorld.h"




DrawableWorld::DrawableWorld(std::size_t size)
	: World(size)
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


	auto& rectArt = g.rectangleArtist;


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

			rectArt->fillRectangle(x * 4, y * 4, 4, 4, color);
		}
	}


	rectArt->endFillRectangle();
}

