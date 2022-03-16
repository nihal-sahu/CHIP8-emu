#pragma once

#include <cstdint>
#include <random>


const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;


class Chip8
{
	public:
		Chip8();
		void loadROM(char const* filename);
		void Cycle();

		uint8_t keypad[KEY_COUNT]{};
		uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};

	private:
		//CHIP8 components
		uint8_t memory[MEMORY_SIZE];
		uint8_t registers[REGISTER_COUNT];
		uint16_t index;
		uint16_t pc;
		uint8_t delayTimer = 0;
		uint8_t soundTimer = 0;
		uint16_t stack[STACK_LEVELS];
		uint8_t sp;
		uint16_t opcode;

		
		void Table0();
		void Table8();
		void TableE();
		void TableF();

		//OPCODES - 34 total
		
		void OP_NULL();
		void OP_00E0();
		void OP_00EE();
		void OP_1nnn();
		void OP_2nnn();
		void OP_3xkk();
		void OP_4xkk();
		void OP_5xy0();
		void OP_6xkk();
		void OP_7xkk();
		void OP_8xy0();
		void OP_8xy1();
		void OP_8xy2();
		void OP_8xy3();
		void OP_8xy4();
		void OP_8xy5();
		void OP_8xy6();
		void OP_8xy7();
		void OP_8xyE();
		void OP_9xy0();
		void OP_Annn();
		void OP_Bnnn();
		void OP_Cxkk();
		void OP_Dxyn();
		void OP_Ex9E();
		void OP_ExA1();
		void OP_Fx07();
		void OP_Fx0A();
		void OP_Fx15();
		void OP_Fx18();
		void OP_Fx1E();
		void OP_Fx29();
		void OP_Fx33();
		void OP_Fx55();
		void OP_Fx65();

		//random number generator
		std::default_random_engine randGen;
		std::uniform_int_distribution<uint8_t> randByte;

		//function tables to execute opcodes
		typedef void (Chip8::*Chip8Func)();
		Chip8Func table[0xF + 1]{&Chip8::OP_NULL};
		Chip8Func table0[0xE + 1]{&Chip8::OP_NULL};
		Chip8Func table8[0xE + 1]{&Chip8::OP_NULL};
		Chip8Func tableE[0xE + 1]{&Chip8::OP_NULL};
		Chip8Func tableF[0x65 + 1]{&Chip8::OP_NULL};
};
