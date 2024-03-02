#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

#include <spdlog/spdlog.h>

#include <cstdint>
#include <map>

namespace chip8::input
{
    class Keyboard
    {
    public:
        Keyboard(sf::Window &w);
        ~Keyboard() = default;

        bool getKeyState(uint8_t key) const noexcept
        {
            return sf::Keyboard::isKeyPressed(m_keysMapping.at(key));
        }

        uint8_t blockUntilKeyPressed();

    private:
        sf::Window &m_window;
        // keys state from 0 -> F
        std::map<uint8_t, sf::Keyboard::Key> m_keysMapping = {
            //
            {0, sf::Keyboard::Num3},
            {1, sf::Keyboard::Num4},
            {2, sf::Keyboard::Num5},
            {3, sf::Keyboard::Num6},
            // --------------------
            {4, sf::Keyboard::E},
            {5, sf::Keyboard::R},
            {6, sf::Keyboard::T},
            {7, sf::Keyboard::Y},
            // --------------------
            {8, sf::Keyboard::D},
            {9, sf::Keyboard::F},
            {0xA, sf::Keyboard::G},
            {0xB, sf::Keyboard::H},
            // --------------------
            {0xC, sf::Keyboard::C},
            {0xD, sf::Keyboard::V},
            {0xE, sf::Keyboard::B},
            {0xF, sf::Keyboard::N}};
    };
}
