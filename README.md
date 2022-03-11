# CHIP8-emu
A chip-8 emulator (interpreter) designed in C++


A breakdown of the traditional CHIP-8 CPU components:
    
    - 16 8-bit Registers (labelled V0 through VF):
        - Each register can hold any value from 0x00 to 0xFF
        - Register VF is a flag to hold info about the results of operations

    - 4 KB of Memory:
        - has the address range of 0x000 to 0xFFF
            - 0x000 - 0x1FF: Reserved for the CHIP-8 interpreter (not used at all in the emulation)
            - 0x050 - 0x0A0: Storage space for the 16 built-in characters (0 through F)
            - 0x200 - 0xFFF: instructions from the ROM are stored here
                - Any space in this range not used by the ROM is free real estate for anything else

    - 16-bit Index Register:
        - a special register to store memory addresses to be used within operations

    - 16-bit Program Counter:
        - a special register that holds the address of the NEXT instruction to execute. 
        - 16-bits in length so that it has the capability to hold the maximum memory address

    - 35 unique opcodes:
        - two bytes in lenght, but addressed only as a single byte.
        - they let us know what instruction to execute when they are fetched
        - after fetching an opcode, the Program Counter (PC) needs to be incremented by 2

    - 16-level stack:
        - helps the CPU to keep track of the order of execution when it calls functions
        - as the name indicates, has 16 unique levels -> so it can hold 16 different PCs.

    - 8-bit Stack Pointer:
        - tells where in the 16-level stack our most recent value was placed
        - since the stack is just represented as an array, the stack pointer will simply be an index of an element within that array
    
    - 8-bit Delay timer:
        - emulates the timer that was included in the original CHIP-8
        - if this timer value is 0, it stays at 0, BUT if it's loaded with a value, it will decrement at a rate of 60Hz
    
    - 8-bit sound timer:
        - has the same behaviour as the delay timer, but instead a single tone will buzz when its value is not zero
    
    - 16 Input Keys:
        - 16 input keys mapped from 0 through F. Each key either holds the value of Pressed or Not Pressed.

    - 64x32 Monochrome Display Memory (screen output): 
        - a memory buffer used for storing graphics to display
        - only has two colours, black or white