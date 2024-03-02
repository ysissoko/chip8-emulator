#include "cpu.hpp"

#include <spdlog/spdlog.h>
#include <fstream>

// Callback for decrementing counters
// uint32_t decrementCounters(uint32_t interval, void *param);

namespace chip8::cpu
{
	CPU::CPU(display::Screen &disp, input::Keyboard &kb) : m_disp{disp}, m_kb{kb}
	{
		initializeJump();
		loadFont();
	}

	void CPU::loadGame(const std::string &gameName)
	{
		std::ifstream game;
		spdlog::debug("game name to open: {}", gameName);
		game.open(gameName, std::ios::in | std::ios::binary);

		if (game.is_open())
		{
			// Get the size of the file
			game.seekg(0, std::ios::end);
			auto fileSize = game.tellg();
			game.seekg(0, std::ios::beg);

			spdlog::info("Loading game file. size {} bytes", fileSize);

			game.read(reinterpret_cast<char *>(m_memory.data() + START_ADDR), fileSize);
			game.close();
		}
		else
		{
			throw std::runtime_error("impossible to load game");
		}
	}

	void CPU::drawSprt(uint8_t b1, uint8_t b2, uint8_t b3)
	{
		// Varie de 1 à 15 pixels
		m_VREG[0xF] = 0;
		auto sprt_h = b1;

		for (auto sprt_row = 0; sprt_row < sprt_h; sprt_row++)
		{
			const auto sprtRowCode = m_memory[m_I + sprt_row];
			const auto ySprt = (m_VREG[b2] + sprt_row) % display::NB_PIXEL_H;
			for (uint8_t sprt_col = 0, dec = 7; sprt_col < display::SPRT_W; sprt_col++, dec--)
			{
				uint8_t xSprt = (m_VREG[b3] + sprt_col) % display::NB_PIXEL_W;
				if ((sprtRowCode & (0x1 << dec)) != 0)
				{
					if (m_disp.getMatrixPixel(ySprt, xSprt))
					{
						m_disp.updatePixelsMatrix(ySprt, xSprt, false);
						m_VREG[0xF] = 1;
					}
					else
					{
						m_disp.updatePixelsMatrix(ySprt, xSprt, true);
					}
				}
			}
		}
	}

	void CPU::initializeJump()
	{
		// OPCODE ID AND OPCODE MASQUE
		jump.msq[0] = 0x0000;
		jump.id[0] = 0x0FFF; /* 0NNN */
		jump.msq[1] = 0xFFFF;
		jump.id[1] = 0x00E0; /* 00E0 */
		jump.msq[2] = 0xFFFF;
		jump.id[2] = 0x00EE; /* 00EE */
		jump.msq[3] = 0xF000;
		jump.id[3] = 0x1000; /* 1NNN */
		jump.msq[4] = 0xF000;
		jump.id[4] = 0x2000; /* 2NNN */
		jump.msq[5] = 0xF000;
		jump.id[5] = 0x3000; /* 3XNN */
		jump.msq[6] = 0xF000;
		jump.id[6] = 0x4000; /* 4XNN */
		jump.msq[7] = 0xF00F;
		jump.id[7] = 0x5000; /* 5XY0 */
		jump.msq[8] = 0xF000;
		jump.id[8] = 0x6000; /* 6XNN */
		jump.msq[9] = 0xF000;
		jump.id[9] = 0x7000; /* 7XNN */
		jump.msq[10] = 0xF00F;
		jump.id[10] = 0x8000; /* 8XY0 */
		jump.msq[11] = 0xF00F;
		jump.id[11] = 0x8001; /* 8XY1 */
		jump.msq[12] = 0xF00F;
		jump.id[12] = 0x8002; /* 8XY2 */
		jump.msq[13] = 0xF00F;
		jump.id[13] = 0x8003; /* BXY3 */
		jump.msq[14] = 0xF00F;
		jump.id[14] = 0x8004; /* 8XY4 */
		jump.msq[15] = 0xF00F;
		jump.id[15] = 0x8005; /* 8XY5 */
		jump.msq[16] = 0xF00F;
		jump.id[16] = 0x8006; /* 8XY6 */
		jump.msq[17] = 0xF00F;
		jump.id[17] = 0x8007; /* 8XY7 */
		jump.msq[18] = 0xF00F;
		jump.id[18] = 0x800E; /* 8XYE */
		jump.msq[19] = 0xF00F;
		jump.id[19] = 0x9000; /* 9XY0 */
		jump.msq[20] = 0xF000;
		jump.id[20] = 0xA000; /* ANNN */
		jump.msq[21] = 0xF000;
		jump.id[21] = 0xB000; /* BNNN */
		jump.msq[22] = 0xF000;
		jump.id[22] = 0xC000; /* CXNN */
		jump.msq[23] = 0xF000;
		jump.id[23] = 0xD000; /* DXYN */
		jump.msq[24] = 0xF0FF;
		jump.id[24] = 0xE09E; /* EX9E */
		jump.msq[25] = 0xF0FF;
		jump.id[25] = 0xE0A1; /* EXA1 */
		jump.msq[26] = 0xF0FF;
		jump.id[26] = 0xF007; /* FX07 */
		jump.msq[27] = 0xF0FF;
		jump.id[27] = 0xF00A; /* FX0A */
		jump.msq[28] = 0xF0FF;
		jump.id[28] = 0xF015; /* FX15 */
		jump.msq[29] = 0xF0FF;
		jump.id[29] = 0xF018; /* FX18 */
		jump.msq[30] = 0xF0FF;
		jump.id[30] = 0xF01E; /* FX1E */
		jump.msq[31] = 0xF0FF;
		jump.id[31] = 0xF029; /* FX29 */
		jump.msq[32] = 0xF0FF;
		jump.id[32] = 0xF033; /* FX33 */
		jump.msq[33] = 0xF0FF;
		jump.id[33] = 0xF055; /* FX55 */
		jump.msq[34] = 0xF0FF;
		jump.id[34] = 0xF065; /* FX65 */
	}

	uint16_t CPU::getCurrentOp()
	{
		return ((m_memory[m_pc] << 8) | m_memory[m_pc + 1]);
	}

	uint16_t CPU::retrieveOpcodeIndex(uint16_t opCode)
	{
		for (uint8_t action = 0; action < NB_OPCODE; action++)
		{
			if ((jump.msq[action] & opCode) == jump.id[action])
				return action;
		}
		return 0;
	}

	void CPU::loadFont()
	{
		m_memory[0] = 0xF0;
		m_memory[1] = 0x90;
		m_memory[2] = 0x90;
		m_memory[3] = 0x90;
		m_memory[4] = 0xF0; // O

		m_memory[5] = 0x20;
		m_memory[6] = 0x60;
		m_memory[7] = 0x20;
		m_memory[8] = 0x20;
		m_memory[9] = 0x70; // 1

		m_memory[10] = 0xF0;
		m_memory[11] = 0x10;
		m_memory[12] = 0xF0;
		m_memory[13] = 0x80;
		m_memory[14] = 0xF0; // 2

		m_memory[15] = 0xF0;
		m_memory[16] = 0x10;
		m_memory[17] = 0xF0;
		m_memory[18] = 0x10;
		m_memory[19] = 0xF0; // 3

		m_memory[20] = 0x90;
		m_memory[21] = 0x90;
		m_memory[22] = 0xF0;
		m_memory[23] = 0x10;
		m_memory[24] = 0x10; // 4

		m_memory[25] = 0xF0;
		m_memory[26] = 0x80;
		m_memory[27] = 0xF0;
		m_memory[28] = 0x10;
		m_memory[29] = 0xF0; // 5

		m_memory[30] = 0xF0;
		m_memory[31] = 0x80;
		m_memory[32] = 0xF0;
		m_memory[33] = 0x90;
		m_memory[34] = 0xF0; // 6

		m_memory[35] = 0xF0;
		m_memory[36] = 0x10;
		m_memory[37] = 0x20;
		m_memory[38] = 0x40;
		m_memory[39] = 0x40; // 7

		m_memory[40] = 0xF0;
		m_memory[41] = 0x90;
		m_memory[42] = 0xF0;
		m_memory[43] = 0x90;
		m_memory[44] = 0xF0; // 8

		m_memory[45] = 0xF0;
		m_memory[46] = 0x90;
		m_memory[47] = 0xF0;
		m_memory[48] = 0x10;
		m_memory[49] = 0xF0; // 9

		m_memory[50] = 0xF0;
		m_memory[51] = 0x90;
		m_memory[52] = 0xF0;
		m_memory[53] = 0x90;
		m_memory[54] = 0x90; // A

		m_memory[55] = 0xE0;
		m_memory[56] = 0x90;
		m_memory[57] = 0xE0;
		m_memory[58] = 0x90;
		m_memory[59] = 0xE0; // B

		m_memory[60] = 0xF0;
		m_memory[61] = 0x80;
		m_memory[62] = 0x80;
		m_memory[63] = 0x80;
		m_memory[64] = 0xF0; // C

		m_memory[65] = 0xE0;
		m_memory[66] = 0x90;
		m_memory[67] = 0x90;
		m_memory[68] = 0x90;
		m_memory[69] = 0xE0; // D

		m_memory[70] = 0xF0;
		m_memory[71] = 0x80;
		m_memory[72] = 0xF0;
		m_memory[73] = 0x80;
		m_memory[74] = 0xF0; // E

		m_memory[75] = 0xF0;
		m_memory[76] = 0x80;
		m_memory[77] = 0xF0;
		m_memory[78] = 0x80;
		m_memory[79] = 0x80; // F
	}

	void CPU::executeOpcode(uint16_t opCode)
	{
		uint8_t b1, b2, b3;

		b1 = (opCode & 0x000F);
		b2 = (opCode & 0x00F0) >> 4;
		b3 = (opCode & 0x0F00) >> 8;

		switch (retrieveOpcodeIndex(opCode))
		{
		case 0:
		{
			// Cet opcode n'est pas implémenté.
			break;
		}
		case 1:
		{
			// 00E0 efface l'écran.
			m_disp.cls();
			break;
		}

		case 2:
		{ // 00EE revient du saut.
			if (cptJump > 0)
			{
				cptJump--;
				m_pc = jmp[cptJump];
			}

			break;
		}
		case 3:
		{ // 1NNN effectue un saut à l'adresse 1NNN.
			m_pc = (b3 << 8) + (b2 << 4) + b1;
			m_pc -= 2;
			break;
		}
		case 4:
		{
			// 2NNN appelle le sous-programme en NNN, mais on revient ensuite.
			jmp[cptJump] = m_pc;
			if (cptJump < NB_JUMP_TOT)
				cptJump++;
			m_pc = (b3 << 8) + (b2 << 4) + b1;
			m_pc -= 2;
			break;
		}
		case 5:
		{
			// 3XNN saute l'instruction suivante si VX est égal à NN.
			if (m_VREG[b3] == ((b2 << 4) + b1))
			{
				m_pc += 2;
			}
			break;
		}
		case 6:
		{ // 4XNN saute l'instruction suivante si VX et NN ne sont pas égaux.
			if (m_VREG[b3] != ((b2 << 4) + b1))
			{
				m_pc += 2;
			}
			break;
		}
		case 7:
		{
			// 5XY0 saute l'instruction suivante si VX et VY sont égaux.
			if (m_VREG[b3] == m_VREG[b2])
			{
				m_pc += 2;
			}

			break;
		}

		case 8:
		{
			// 6XNN définit VX à NN.
			m_VREG[b3] = (b2 << 4) + b1;
			break;
		}
		case 9:
		{
			// 7XNN ajoute NN à VX.
			m_VREG[b3] += (b2 << 4) + b1;

			break;
		}
		case 10:
		{
			// 8XY0 définit VX à la valeur de VY.
			m_VREG[b3] = m_VREG[b2];

			break;
		}
		case 11:
		{
			// 8XY1 définit VX à VX OR VY.
			m_VREG[b3] = m_VREG[b3] | m_VREG[b2];
			break;
		}
		case 12:
		{
			// 8XY2 définit VX à VX AND VY.
			m_VREG[b3] = m_VREG[b3] & m_VREG[b2];
			break;
		}
		case 13:
		{
			// 8XY3 définit VX à VX XOR VY.
			m_VREG[b3] = m_VREG[b3] ^ m_VREG[b2];
			break;
		}
		case 14:
		{
			// 8XY4 ajoute VY à VX. VF est mis à 1 quand il y a un dépassement de mémoire (carry), et à 0 quand il n'y en pas.
			if ((m_VREG[b3] + m_VREG[b2]) > 0xFF)
			{
				m_VREG[0xF] = 1; // V[15]
			}
			else
			{
				m_VREG[0xF] = 0; // V[15]
			}
			m_VREG[b3] += m_VREG[b2];

			break;
		}
		case 15:
		{
			// 8XY5 VY est soustraite de VX. VF est mis à 0 quand il y a un emprunt, et à 1 quand il n'y a en pas.
			if ((m_VREG[b3] < m_VREG[b2]))
			{
				m_VREG[0xF] = 0; // m_VREG[15]
			}
			else
			{
				m_VREG[0xF] = 1; // m_VREG[15]
			}
			m_VREG[b3] -= m_VREG[b2];

			break;
		}
		case 16:
		{

			// 8XY6 décale (shift) VX à droite de 1 bit. VF est fixé à la valeur du bit de poids faible de VX avant le décalage.
			m_VREG[0xF] = (m_VREG[b3] & (0x01));
			m_VREG[b3] = (m_VREG[b3] >> 1);
			break;
		}
		case 17:
		{
			// 8XY7 VX = VY - VX. VF est mis à 0 quand il y a un emprunt et à 1 quand il n'y en a pas.
			if ((m_VREG[b2] < m_VREG[b3])) // /!\ VF est mis à 0 quand il y a emprunt !
			{
				m_VREG[0xF] = 0; // m_VREG[15]
			}
			else
			{
				m_VREG[0xF] = 1; // m_VREG[15]
			}

			m_VREG[b3] = m_VREG[b2] - m_VREG[b3];
			break;
		}
		case 18:
		{
			// 8XYE décale (shift) VX à gauche de 1 bit. VF est fixé à la valeur du bit de poids fort de VX avant le décalage.
			m_VREG[0xF] = (m_VREG[b3] >> 7);
			m_VREG[b3] = (m_VREG[b3] << 1);
			break;
		}

		case 19:
		{

			// 9XY0 saute l'instruction suivante si VX et VY ne sont pas égaux.
			if (m_VREG[b3] != m_VREG[b2])
			{
				m_pc += 2;
			}
			break;
		}
		case 20:
		{
			// ANNN affecte NNN à I.
			m_I = (b3 << 8) + (b2 << 4) + b1;
			break;
		}
		case 21:
		{
			// BNNN passe à l'adresse NNN + V0.

			m_pc = (b3 << 8) + (b2 << 4) + b1 + m_VREG[0];
			m_pc -= 2;

			break;
		}
		case 22:
		{

			// CXNN définit VX à un nombre aléatoire inférieur à NN.
			m_VREG[b3] = (rand()) % ((b2 << 4) + b1 + 1);

			break;
		}

		case 23:
		{
			// DXYN dessine un sprite aux coordonnées (VX, VY).
			drawSprt(b1, b2, b3);
			break;
		}
		case 24:
		{
			// EX9E saute l'instruction suivante si la clé stockée dans VX est pressée.
			if (m_kb.getKeyState(m_VREG[b3]) == true) // 1 = pressé ; 0 = relâché
			{
				m_pc += 2;
			}
			break;
		}
		case 25:
		{
			// EXA1 saute l'instruction suivante si la clé stockée dans VX n'est pas pressée.
			if (m_kb.getKeyState(m_VREG[b3]) == false) // 1 = pressé ; 0 = relâché
			{
				m_pc += 2;
			}
			break;
		}

		case 26:
		{
			// FX07 définit VX à la valeur de la temporisation.
			m_VREG[b3] = m_syncCounter;
			break;
		}
		case 27:
		{
			// FX0A attend l'appui sur une touche et la stocke ensuite dans VX.
			waitForKeyPress(b3);
			break;
		}

		case 28:
		{
			// FX15 définit la temporisation à VX.
			m_syncCounter = m_VREG[b3];

			break;
		}
		case 29:
		{
			// FX18 définit la minuterie sonore à VX.
			m_soundCounter = m_VREG[b3];

			break;
		}
		case 30:
		{
			// FX1E ajoute à VX I. VF est mis à 1 quand il y a overflow (I+VX>0xFFF), et à 0 si tel n'est pas le cas.
			if ((m_I + m_VREG[b3]) > 0xFFF)
			{
				m_VREG[0xF] = 1;
			}
			else
			{
				m_VREG[0xF] = 0;
			}
			m_I += m_VREG[b3];

			break;
		}

		case 31:
		{
			// FX29 définit I à l'emplacement du caractère stocké dans VX. Les caractères 0-F (en hexadécimal) sont représentés par une police 4x5.
			m_I = 5 * m_VREG[b3];
			break;
		}

		case 32:
		{
			// FX33 stocke dans la mémoire le code décimal représentant VX (dans I, I+1, I+2).
			m_memory[m_I] = (m_VREG[b3] - m_VREG[b3] % 100) / 100;						   // stocke les centaines
			m_memory[m_I + 1] = (((m_VREG[b3] - m_VREG[b3] % 10) / 10) % 10);			   // les dizaines
			m_memory[m_I + 2] = m_VREG[b3] - m_memory[m_I] * 100 - 10 * m_memory[m_I + 1]; // les unités
			break;
		}
		case 33:
		{

			// FX55 stocke V0 à VX en mémoire à partir de l'adresse I.

			for (uint8_t i = 0; i <= b3; i++)
			{
				m_memory[m_I + i] = m_VREG[i];
			}

			break;
		}
		case 34:
		{
			// FX65 remplit V0 à VX avec les valeurs de la mémoire à partir de l'adresse I.

			for (uint8_t i = 0; i <= b3; i++)
			{
				m_VREG[i] = m_memory[m_I + i];
			}

			break;
		}

		default:
		{ // si ça arrive, il y un truc qui cloche
			spdlog::error("OpCode UNKNOWN");
			break;
		}
		}

		m_pc += 2;
	}

	void CPU::waitForKeyPress(uint8_t b3)
	{
		spdlog::info("waiting key input");
		m_VREG[b3] = m_kb.blockUntilKeyPressed();
	}

	void CPU::run()
	{
		auto currentOp = getCurrentOp();
		executeOpcode(currentOp);
		decrementCounters();
	}

}
