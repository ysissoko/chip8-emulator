#include "cpu/cpu.hpp"
#include "display/screen.hpp"

#include <SFML/Graphics.hpp>

#include <spdlog/spdlog.h>

#include <filesystem>
#include <boost/program_options.hpp>

#include <memory>
#include <fstream>
#include <filesystem>

namespace po = boost::program_options;

using namespace std;
using namespace chip8;

int main(int ac, char **av)
{
	spdlog::set_level(spdlog::level::debug);
	// Declare the options description
	po::options_description desc("Allowed options");
	desc.add_options()("help", "produce help message")("rom", po::value<std::string>(), "ROM name");

	// Declare a variables map to store parsed options
	po::variables_map vm;
	po::store(po::parse_command_line(ac, av, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		spdlog::info("some help");
		return EXIT_SUCCESS;
	}

	if (!vm.count("rom"))
	{
		spdlog::error("Enter the rom name parameter on the CLI");
		return EXIT_FAILURE;
	}

	std::filesystem::path path{"roms"};
	path /= std::filesystem::path(vm["rom"].as<std::string>());

	// Create a window
	sf::RenderWindow window(sf::VideoMode(display::SCREEN_W, display::SCREEN_H), "CHIP 8 EMULATOR (by Youssouf SISSOKO)");

	display::Screen disp(window);
	input::Keyboard kb(window);

	chip8::cpu::CPU cpu(disp, kb);
	cpu.loadGame(path.string());

	// Initialize clock
	sf::Clock clock;
	sf::Time refreshRate = sf::milliseconds(1); // 500 Hz

	// Loop

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Check if it's time for the next iteration
		if (clock.getElapsedTime() >= refreshRate)
		{
			window.clear();
			// Reset clock for next iteration
			clock.restart();
			cpu.run();				 // step to execute the next opcode
			disp.showPixelsMatrix(); // refresh the screen
			window.display();
		}
	}

	return EXIT_SUCCESS;
}
