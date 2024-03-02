#pragma once

#include "src/display/screen.hpp"
#include "src/input/keyboard.hpp"

#include <stack>

namespace chip8::cpu
{
	constexpr uint8_t NB_OPCODE = 35;
	constexpr uint16_t MEMORY_SIZE = 4096;
	constexpr uint16_t START_ADDR = 512;
	constexpr uint8_t NB_REGISTERS = 16;
	constexpr uint8_t COUNTER_DELAY = 16; // 1/60 hertz
	constexpr uint8_t CPU_TIMING = 4;
	constexpr uint8_t NB_JUMP_TOT = 15;

	typedef struct
	{
		uint16_t id[NB_OPCODE];
		uint16_t msq[NB_OPCODE];
	} Jump;

	class CPU
	{
	public:
		CPU(display::Screen &, input::Keyboard &);
		~CPU() = default;
		void loadGame(const std::string &gameName);

		void run();

	private:
		std::array<uint8_t, NB_REGISTERS> m_VREG = {0};
		std::array<uint8_t, MEMORY_SIZE> m_memory = {0};
		std::array<uint16_t, NB_JUMP_TOT> jmp = {0};

		uint16_t m_I{0};
		uint16_t m_pc{START_ADDR};

		std::stack<uint16_t> m_stack;

		uint8_t cptJump{0};
		uint8_t m_syncCounter{0};
		uint8_t m_soundCounter{0};

		display::Screen &m_disp;
		input::Keyboard &m_kb;

		Jump jump;
		bool isRunning;

		// Private methods
		void initializeJump();
		void loadFont();
		uint16_t retrieveOpcodeIndex(uint16_t opCode);
		void drawSprt(uint8_t b1, uint8_t b2, uint8_t b3);
		void waitForKeyPress(uint8_t);
		void executeOpcode(uint16_t opCode);
		uint16_t getCurrentOp();

		void decrementCounters()
		{
			m_soundCounter--;
			m_syncCounter--;
		}
	};

}
