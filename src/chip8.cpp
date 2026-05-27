#include "chip8.hpp"
#include <cstdlib> 
#include <ctime>  
#include <iostream>

const uint8_t chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8(){
    srand(static_cast<unsigned int>(time(NULL)));

    
    program_counter = 0x200;
    opcode = 0;
    index = 0;
    stack_pointer = 0;
    delay_timer = 0;
    sound_timer = 0;

    for(uint8_t &x : graphics){
        x = 0;
    }
    for(uint8_t &x : memory){
        x = 0;
    }
    for(uint16_t &x : stack){
        x = 0;
    }
    for(uint8_t &x : registers){
        x = 0;
    }
    for(uint8_t &x : keys){
        x = 0;
    }
    for(int i = 0; i < 80; ++i){
        uint8_t character = chip8_fontset[i];
        memory[i] = character;
    }
}

void Chip8::increment_program_counter(){
        program_counter += 2; // because every instruction is 2 bytes
}
void Chip8::cycle(){
   
    //since instructions are 2 bytes wide, take the instruction (eg. 10010001),
    //left shift (1001000100000000)
    //append the byte right after program counter index (e.g. 1001000101010101)
    opcode = memory[program_counter] << 8 | memory[program_counter + 1];
    std::cout << opcode;
    int first = opcode >> 12;
    int x = 0;
    int y = 0;
    int kk = 0;
    switch(first){
        
        case 0x0:
            if(opcode == 0x00E0){
                // clear screen
                for(uint8_t& g: graphics){
                    g = 0;
                }
            } else if(opcode == 0x00EE){
                --stack_pointer;
                program_counter = stack[stack_pointer];
            }
            increment_program_counter();
            break;
        case 0x1:
            program_counter = opcode & 0xFFF; //keeps only the rightmost 12 bits
            break;
        case 0x2:
            //store current instruction on the stack, prepare to run different instruction
            stack[stack_pointer] = program_counter;
            ++stack_pointer;
            program_counter = opcode & 0xFFF;
            break;
        case 0x3:
            x = (opcode & 0xF00) >> 8;
            if (registers[x] == (opcode & 0x0FF)){
                increment_program_counter();
            }
            increment_program_counter();
            break;
        case 0x4:
            x = (opcode & 0xF00) >> 8;
            if (registers[x] != (opcode & 0x0FF) ){
                increment_program_counter();
            }
            increment_program_counter();
            break;
        case 0x5:
            x = (opcode & 0xF00) >> 8;
            y = (opcode & 0x0F0) >> 4;
            if(registers[x] == registers[y]){
                increment_program_counter();
            }
            increment_program_counter();
            break;
        case 0x6:
            x = (opcode & 0xF00) >> 8;
            registers[x] = opcode & 0x0FF;
            increment_program_counter();
            break;
        case 0x7:
            x = (opcode & 0xF00) >> 8;
            registers[x] += opcode & 0x0FF;
            increment_program_counter();
            break;
        case 0x8:
            x = (opcode & 0xF00) >> 8;
            y = (opcode & 0x0F0) >> 4;
            switch(opcode & 0x000F){
                case 0x0:
                    registers[x] = registers[y];
                    break;
                case 0x1:
                    registers[x] = registers[x] | registers[y];
                    break;
                case 0x2:
                    registers[x] = registers[x] & registers[y];
                    break;
                case 0x3:
                    registers[x] = registers[x] ^ registers[y];
                    break;
                case 0x4: {
                    uint16_t sum = registers[x] + registers[y];
                    registers[0xF] = sum > 255 ? 1 : 0;
                    registers[x] = sum & 0x00FF;
                    break;
                }
                case 0x5:
                    registers[0xF] = registers[x] > registers[y] ? 1 : 0;
                    registers[x] = registers[x] - registers[y];
                    break;  
                case 0x6:
                    registers[0xF] = registers[x] & 1;
                    registers[x] =  registers[x] >> 1;
                    break;
                case 0x7:
                    registers[0xF] = registers[x] < registers[y] ? 1 : 0;
                    registers[x] = registers[y] - registers[x];
                    break;  
                case 0xE:
                    //we want the most significant bit and 0x80 == 1000 0000
                    registers[0xF] = ((registers[x] & 0x80) != 0)? 1 : 0;
                    registers[x] =  registers[x] << 1;
                    break;
                

            }
           
            increment_program_counter();
            break;

        case 0x9:
            x = (opcode & 0xF00) >> 8;
            y = (opcode & 0x0F0) >> 4;
            if(registers[x] != registers[y]){
                increment_program_counter();
            }
            increment_program_counter();
            break;
        case 0xA:
            index = opcode & 0x0FFF;
            increment_program_counter();
            break;
        case 0xB:
            program_counter = (opcode & 0xFFF) + static_cast<uint16_t>(registers[0]);
            break;
        case 0xC:
            x = (opcode & 0xF00) >> 8;
            kk = opcode& 0x0FF;
            registers[x]= static_cast<uint8_t> (static_cast<uint32_t>(rand()) & kk);
            increment_program_counter();
            break;
        case 0xD: {
            registers[0xF] = 0;
            int xx = (opcode & 0x0F00) >> 8;
            int yy = (opcode & 0x00F0) >> 4;
            int nn = (opcode & 0x000F);
            int reg_x = registers[xx];
            int reg_y = registers[yy];
            y = 0;
            while(y < nn){
                int pixel = memory[index + y];
                x = 0;
                //sprites are always 8 pixels wide
                while(x < 8){
                    const u_int8_t msb = 0x80;

                    if((pixel & (msb >> x)) != 0){
                        int t_x = (reg_x + x) % 64;
                        int t_y = (reg_y + y) % 32;
                        int i = t_x + t_y * 64;
                        graphics[i] = graphics[i] ^ 1;
                        if(graphics[i] == 0){
                            registers[0xF] = 1;
                        }
                    }
                    ++x;
                }

                ++y;
            }
            increment_program_counter();
            break;
        }
        case 0xE:
            x = (opcode & 0xF00) >> 8;
            kk = opcode& 0x0FF;
            if(kk == 0x9E){
                if(keys[registers[x]] == 1){
                    increment_program_counter();
                }
            } 
            else if(kk == 0xA1){
                if(keys[registers[x]] != 1){
                    increment_program_counter();
                }
            }
            increment_program_counter();
        
            break;
        case 0xF:
            x = (opcode & 0xF00) >> 8;
            kk = opcode& 0x0FF;
            switch(kk){
                case 0x07:
                    registers[x] = delay_timer;
                    break;
                case 0x0A: {
                    bool key_pressed  = false;
                    for(int i = 0; i < 16; ++i){
                        if(keys[i] != 0){
                            registers[x] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    //keep rerunning the same cycle until the key is pressed.
                    if(!key_pressed) {
                        program_counter -= 2; 
                        return;
                    }

                    break;
                }
                case 0x15:
                    delay_timer = registers[x];
                    break;
                case 0x18:
                    sound_timer = registers[x];
                    break;
                case 0x1E:
                    index = index + registers[x];
                    break;
                case 0x29:
                    if(registers[x] < 16){
                        
                        index = registers[x] * 0x5;
                    }
                    break;
                case 0x33:
                    memory[index] = registers[x] / 100;
                    memory[index + 1] = (registers[x] / 10) % 10;
                    memory[index + 2] = registers[x] % 10;
                    break;
                case 0x55:
                    for(int i = 0; i <= x; ++i){
                        memory[index + i] = registers[i];
                    }
                    break;
                case 0x65:
                    for(int i = 0; i <= x; ++i){
                        registers[i] = memory[index + i];
                    }
                    break;

            }

            increment_program_counter();
        
        default:
            break;


    }
    if(delay_timer > 0){
        --delay_timer;
    }
    if(sound_timer > 0){
        --sound_timer;
    }


}