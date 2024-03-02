#include "keyboard.hpp"

#include <SFML/Window/Event.hpp>
#include <boost/optional.hpp>

namespace chip8::input
{
    Keyboard::Keyboard(sf::Window &w) : m_window{w}
    {
    }

    uint8_t Keyboard::blockUntilKeyPressed()
    {
        sf::Event event;
        boost::optional<uint8_t> keyPressed{boost::none};

        while (keyPressed == boost::none)
        {

            m_window.waitEvent(event);
            if (event.type == sf::Event::KeyPressed)
            {
                for (const auto &pair : m_keysMapping)
                {
                    if (pair.second == event.key.code)
                        keyPressed = pair.first;
                }
            }
        }

        return keyPressed.get();
    }
}
