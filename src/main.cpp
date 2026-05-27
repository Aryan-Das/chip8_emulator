#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "chip8.hpp"


using namespace std;

SDL_Window* win;
SDL_Texture* texture;
SDL_Renderer* renderer;
Chip8* cpu;

const int keymap[]{
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E, 
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V
};


void init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw string("error initializing SDL: %s\n", SDL_GetError());
        
    }
    win = SDL_CreateWindow("EMULATOR", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    700, 700, 0);
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;


    renderer = SDL_CreateRenderer(win, -1, render_flags);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    cpu = new Chip8();
}
void de_init(){
    SDL_DestroyWindow(win);
    
   
    SDL_Quit();
    delete cpu;
}
void load_rom(char* filename){
    ifstream input_file(filename);
    uintmax_t size = filesystem::file_size(filename);
    char byte;
    int i = 0;
    while (input_file.get(byte)) {
        cpu->memory[i + 0x200] = byte;
        ++i;
    }
}


int main(int argc, char *argv[]){
    
    
    init();
    // for (int i = 1; i < argc; ++i) {
    //    argv[i];
    // }
    char* filename = argv[1];
    if (!filename){
        throw(string("No ROM Provided!\n"));
    }
    load_rom(filename);

    

    
    bool keep_open = true;

    while(keep_open){
        cpu->cycle();
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0){
            switch (e.type) {
                case SDL_QUIT:
                    keep_open = false;
                    break;
                case SDL_KEYDOWN: {
                    for(int i = 0; i < 16; ++i){
                        if(e.key.keysym.scancode == keymap[i]){
                            cpu->keys[i]= 1;

                        }
                    }
                    break;
                    
                }
                case SDL_KEYUP: {
                    for(int i = 0; i < 16; ++i){
                        if(e.key.keysym.scancode == keymap[i]){
                            cpu->keys[i]= 0;
                            
                        }
                    }
                    break;
                    
                }

                default:
                    break;
            }

        }
        SDL_RenderClear(renderer);
        //Update Texture
        void* bytes = nullptr;
        int pitch = 0;

        SDL_LockTexture(texture, nullptr, &bytes, &pitch);
        uint32_t* pixelBuffer = static_cast<uint32_t*>(bytes);

        for(int i = 0; i < 2048; ++i){
            pixelBuffer[i] = (cpu->graphics[i] == 1) ? 0xFFFFFFFF : 0x000000FF;
        }
        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
        
    
    de_init();
}