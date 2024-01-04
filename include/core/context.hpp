#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class Context {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    Context();
    ~Context();
};