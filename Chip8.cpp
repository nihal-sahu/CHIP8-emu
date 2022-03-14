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

	if (V[Vx] == byte)
	{
		pc += 2;
	}
}

void Chip8::OP_4xkk()       //skips the next instruction if Vx != kk
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (V[Vx] != byte)
	{
		pc += 2;
	}
}

void Chip8::OP_5xy0()       //skips the next instruction if Vx = Vy
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] == V[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_6xkk()       //sets Vx = kk
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = byte;
}

void Chip8::OP_7xkk()       //sets Vx = Vx + kk
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] += byte;
}

void Chip8::OP_8xy0()       //sets Vx = Vy
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] = V[Vy];
}

void Chip8::OP_8xy1()       //sets Vx=Vx OR Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    V[Vx] |= V[Vy];
}

void Chip8::OP_8xy2()       //sets Vx=Vx AND Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3()       //sets Vx=Vx XOR Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8xy4()       //sets Vx=Vx+Vy and sets Vf as carry flag
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = registers[Vx] + registers[Vy];

    if (sum > 255u)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }

    registers[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5()       //sets Vx=Vx-Vy and sets Vf to not borrow
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] > registers[Vy])
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }

    registers[Vx] -= registers[Vy];
}

void Chip8::OP_8xy6()   //sets  Vx=Vx SHR 1
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = (registers[Vx] & 0x1u);

    registers[Vx] >>= 1;
}

void Chip8::OP_8xy7()       //sets Vx=Vy-Vx, and Vf to NOT BORROW
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vy] > registers[Vx])
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }

    registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8xyE()       //sets Vx =Vx SHL 1
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

    registers[Vx] <<= 1;
}

void Chip8::OP_9xy0()       //skips next instruction if Vx != Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] != registers[Vy])
    {    
        pc += 2;
    }
}

void Chip8::OP_Annn()       //sets I to nnn
{
    uint16_t address = opcode & 0x0FFFu;

    index = address;
}

void Chip8::OP_Bnnn()       //jumps to location nnn + V0
{
    uint16_t address = opcode & 0x0FFFu;

    pc = registers[0] + address;
}

void Chip8::OP_Cxkk()       //sets Vx = random byte and kk
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    registers[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn()       //displays a n-byte sprite starting at memory location I at (Vx, Vy), and sets VF flag to collison
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint8_t height = opcode & 0x000Fu;

    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (uint16_t row = 0; row < height; ++row)
    {
        uint8_t spriteByte = memory[index + row];

        for (uint16_t col = 0; col < 8; ++col)
        {
            uint8_t spritePixel = spriteByte & (ox80u >> col);
            uint32_t *screenPixel = &gfx[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            if (spritePixel)
            {
                if (*screenPixel = 0xFFFFFFFF)
                {
                    registers[0xF] = 1;
                }

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}


