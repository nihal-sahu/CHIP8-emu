#include <fstream>

#include "Chip8.h"

Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    pc = START_ADDRESS;

    //loading the fonts into memory
    for (uint16_t i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    //initiailizing our random number generator (generating a number between 0 and 255)
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

Chip8::~Chip8()
{

}

void Chip8::loadROM(char const* filename)
{
    //open file as a stream of binary
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        //get the file size
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        //return to beginning of file, fill the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        //load ROM contents into memory, starting at the starting address
        for (uint32_t i = 0; i < size; ++i)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }

        //deallocate the buffer
        delete[] buffer;
        buffer = nullptr;
    }
}

//OPCODES
void Chip8::OP_00E0()       //clears the display
{
    memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE()       //returns from a subroutine
{
    --sp;
    pc = stack[sp];
}

void Chip8::OP_1nnn()       //jumps to a specific memory address
{
	uint16_t address = opcode & 0x0FFFu;

	pc = address;
}

void Chip8::OP_2nnn()       //calls a subroutine at a specific memory address
{
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc;
    ++sp;
    pc = address;
}

void Chip8::OP_3xkk()       //skips the next instruction if Vx = kk
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] == byte)
	{
		pc += 2;
	}
}

void Chip8::OP_4xkk()       //skips the next instruction if Vx != kk
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] != byte)
	{
		pc += 2;
	}
}

void Chip8::OP_5xy0()       //skips the next instruction if Vx = Vy
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_6xkk()       //sets Vx = kk
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = byte;
}

void Chip8::OP_7xkk()       //sets Vx = Vx + kk
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] += byte;
}

void Chip8::OP_8xy0()       //sets Vx = Vy
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vy];
}