#include "screen.hpp"

using namespace std;

namespace chip8::display
{
	Screen::Screen(sf::RenderWindow &window) : m_window{window}
	{
		cls();
	}

	void Screen::cls()
	{
		for (int col = 0; col < NB_PIXEL_W; col++)
		{
			for (int row = 0; row < NB_PIXEL_H; row++)
			{
				pixelMatrix[col][row] = false;
			}
		}
	}

	void Screen::showPixelsMatrix()
	{
		for (auto col = 0; col < NB_PIXEL_W; col++)
		{
			for (auto row = 0; row < NB_PIXEL_H; row++)
			{
				if (pixelMatrix[col][row] == true)
				{
					// Create a rectangle shape
					sf::RectangleShape pixelRect(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE)); // Width = 100, Height = 50
					pixelRect.setFillColor(sf::Color::White);
					pixelRect.setPosition(col * PIXEL_SIZE, row * PIXEL_SIZE); // Position the rectangle
					m_window.draw(pixelRect);
				}
			}
		}
	}

	void Screen::updatePixelsMatrix(uint8_t row, uint8_t col, bool isActive)
	{
		pixelMatrix[col][row] = isActive;
	}

}
