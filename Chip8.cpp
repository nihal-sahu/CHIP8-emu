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

    V[Vx] &= V[Vy];
}

void Chip8::OP_8xy3()       //sets Vx=Vx XOR Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    V[Vx] ^= V[Vy];
}

void Chip8::OP_8xy4()       //sets Vx=Vx+Vy and sets Vf as carry flag
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = V[Vx] + V[Vy];

    if (sum > 255u)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }

    V[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5()       //sets Vx=Vx-Vy and sets Vf to not borrow
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (V[Vx] > V[Vy])
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }

    V[Vx] -= V[Vy];
}

void Chip8::OP_8xy6()   //sets  Vx=Vx SHR 1
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    V[0xF] = (V[Vx] & 0x1u);

    V[Vx] >>= 1;
}

void Chip8::OP_8xy7()       //sets Vx=Vy-Vx, and Vf to NOT BORROW
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (V[Vy] > V[Vx])
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }

    V[Vx] = V[Vy] - V[Vx];
}

void Chip8::OP_8xyE()       //sets Vx =Vx SHL 1
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    V[0xF] = (V[Vx] & 0x80u) >> 7u;

    V[Vx] <<= 1;
}

void Chip8::OP_9xy0()       //skips next instruction if Vx != Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (V[Vx] != V[Vy])
    {    
        pc += 2;
    }
}

void Chip8::OP_Annn()       //sets I to nnn
{
    uint16_t address = opcode & 0x0FFFu;

    I = address;
}

void Chip8::OP_Bnnn()       //jumps to location nnn + V0
{
    uint16_t address = opcode & 0x0FFFu;

    pc = V[0] + address;
}

void Chip8::OP_Cxkk()       //sets Vx = random byte and kk
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn()       //displays a n-byte sprite starting at memory location I at (Vx, Vy), and sets VF flag to collison
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint8_t height = opcode & 0x000Fu;

    uint8_t xPos = V[Vx] % VIDEO_WIDTH;
    uint8_t yPos = V[Vy] % VIDEO_HEIGHT;

    V[0xF] = 0;

    for (uint16_t row = 0; row < height; ++row)
    {
        uint8_t spriteByte = memory[I + row];

        for (uint16_t col = 0; col < 8; ++col)
        {
            uint8_t spritePixel = spriteByte & (ox80u >> col);
            uint32_t *screenPixel = &gfx[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            if (spritePixel)
            {
                if (*screenPixel = 0xFFFFFFFF)
                {
                    V[0xF] = 1;
                }

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::OP_Ex9E()       //skips next instruction if key with the value of Vx is pressed
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (keypad[key])
	{
		pc += 2;
	}
}

void Chip8::OP_ExA1()       //skips next instruction if key with the value of Vx is not pressed
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (!keypad[key])
	{
		pc += 2;
	}
}

void Chip8::OP_Fx07()       //sets Vx to the value of the delay timer
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	V[Vx] = delayTimer;
}

void Chip8::OP_Fx0A()       //stores the value of a pressed key in Vx
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[0])
	{
		V[Vx] = 0;
	}
	else if (keypad[1])
	{
		V[Vx] = 1;
	}
	else if (keypad[2])
	{
		V[Vx] = 2;
	}
	else if (keypad[3])
	{
		V[Vx] = 3;
	}
	else if (keypad[4])
	{
		V[Vx] = 4;
	}
	else if (keypad[5])
	{
		V[Vx] = 5;
	}
	else if (keypad[6])
	{
		V[Vx] = 6;
	}
	else if (keypad[7])
	{
		V[Vx] = 7;
	}
	else if (keypad[8])
	{
		V[Vx] = 8;
	}
	else if (keypad[9])
	{
		V[Vx] = 9;
	}
	else if (keypad[10])
	{
		V[Vx] = 10;
	}
	else if (keypad[11])
	{
		V[Vx] = 11;
	}
	else if (keypad[12])
	{
		V[Vx] = 12;
	}
	else if (keypad[13])
	{
		V[Vx] = 13;
	}
	else if (keypad[14])
	{
		V[Vx] = 14;
	}
	else if (keypad[15])
	{
		V[Vx] = 15;
	}
	else
	{
		pc -= 2;
	}
}

void Chip8::OP_Fx15()       //sets the delay timer equal to Vx
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = V[Vx];
}

void Chip8::OP_Fx18()       //sets the sound timer equal to Vx
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = V[Vx];
}

void Chip8::OP_Fx1E()       //sets index=index + Vx
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	I += V[Vx];
}

void Chip8::OP_Fx29()       //sets index equal to the location of the sprite for digit Vx
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = V[Vx];

	I = FONTSET_START_ADDRESS + (5 * digit);
}

void Chip8::OP_Fx33()       //stores BCD representation of Vx in 3 different memory locations (based on index)
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = V[Vx];

	// Ones-place
	memory[I + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[I + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[I] = value % 10;
}

void Chip8::OP_Fx55()       //stores registers V0 through Vx in memory starting at the specified index 
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[I + i] = V[i];
	}
}

void Chip8::OP_Fx65()       //reads registers V0 through Vx starting at specified index
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		V[i] = memory[I + i];
	}
}