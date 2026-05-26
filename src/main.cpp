#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "chip8.hpp"

using namespace std;

SDL_Window* win;
SDL_Texture* texture;
SDL_Renderer* renderer;
Chip8* cpu;

void init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw string("error initializing SDL: %s\n", SDL_GetError());
        
    }
    win = SDL_CreateWindow("EMULATOR", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    1000, 1000, 0);
    
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


int main(int argc, char *argv[]){
    
    
    init();
    

    
    bool keep_open = true;

    while(keep_open){
        cpu->cycle();
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0){
            switch (e.type) {
                case SDL_QUIT:
                    keep_open = false;
                    break;
                default:
                    break;
            }

        }
        SDL_RenderClear(renderer);
        //Update Texture

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
        
    
    de_init();
}