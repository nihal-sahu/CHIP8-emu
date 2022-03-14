#include <cstdint>
#include <chrono>
#include <random>

/* MEMORY MAP OF CHIP 8
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/

class Chip8
{
    public:
        Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count());
        ~Chip8();

    
    private:
        //some constants
        const uint16_t START_ADDRESS = 0x200;
        const uint16_t FONTSET_SIZE = 80;
        const uint16_t FONTSET_START_ADDRESS = 0x50;

        //each row in this array represents an input 0 - F
        uint8_t fontset[FONTSET_SIZE] =     
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, 
            0x20, 0x60, 0x20, 0x20, 0x70, 
            0xF0, 0x10, 0xF0, 0x80, 0xF0, 
            0xF0, 0x10, 0xF0, 0x10, 0xF0, 
            0x90, 0x90, 0xF0, 0x10, 0x10, 
            0xF0, 0x80, 0xF0, 0x10, 0xF0, 
            0xF0, 0x80, 0xF0, 0x90, 0xF0, 
            0xF0, 0x10, 0x20, 0x40, 0x40, 
            0xF0, 0x90, 0xF0, 0x90, 0xF0, 
            0xF0, 0x90, 0xF0, 0x10, 0xF0, 
            0xF0, 0x90, 0xF0, 0x90, 0x90, 
            0xE0, 0x90, 0xE0, 0x90, 0xE0, 
            0xF0, 0x80, 0x80, 0x80, 0xF0, 
            0xE0, 0x90, 0x90, 0x90, 0xE0, 
            0xF0, 0x80, 0xF0, 0x80, 0xF0, 
            0xF0, 0x80, 0xF0, 0x80, 0x80  
        };

        std::default_random_engine randGen;
	    std::uniform_int_distribution<uint8_t> randByte;
    
    public:
        //CHIP8 components
        uint8_t V[16];
        uint8_t memory[4096];
        
        uint16_t I;
        uint16_t pc;

        uint16_t stack[16];
        uint8_t sp;

        uint8_t delayTimer;
	    uint8_t soundTimer;
	    
        uint8_t keypad[16];
	    uint32_t gfx[64 * 32];
	    
        uint16_t opcode;

    public:
        //CHIP8 Opcodes
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

    public:
        // CHIP8 functions
        void loadROM(char const *filename);
};      