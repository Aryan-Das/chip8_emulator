#include <cstdint>



class Chip8{
public:
    Chip8();   
    
    void increment_program_counter();

    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t graphics[2048];
    uint8_t registers[16];
    uint16_t index;
    uint16_t program_counter;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint16_t stack_pointer;
    uint8_t keys[16];
    
};