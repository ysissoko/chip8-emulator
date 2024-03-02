#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <memory>

namespace chip8::display
{

	constexpr uint8_t PIXEL_SIZE = 8;

	// CHIP-8 RESOLUTION 64*32 PIXELS
	constexpr uint8_t NB_PIXEL_W = 64;
	constexpr uint8_t NB_PIXEL_H = 32;
	constexpr uint8_t FPS_RATE = 16;
	constexpr uint8_t SPRT_W = 8;

	const uint16_t SCREEN_W = PIXEL_SIZE * NB_PIXEL_W;
	const uint16_t SCREEN_H = PIXEL_SIZE * NB_PIXEL_H;

	class Screen
	{
	public:
		Screen(sf::RenderWindow &renderer);
		~Screen() = default;

		void showPixelsMatrix();
		void updatePixelsMatrix(uint8_t row, uint8_t col, bool isActive);
		bool getMatrixPixel(uint8_t row, uint8_t col) const noexcept { return pixelMatrix[col][row]; }
		void cls();

	private:
		std::array<std::array<bool, NB_PIXEL_H>, NB_PIXEL_W> pixelMatrix;
		sf::RenderWindow &m_window;
	};
}
