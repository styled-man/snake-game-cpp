#include "core/context.hpp"

Context::Context() {
    // initilize sdl
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize subsystems: %s", SDL_GetError());
        return;
    }
    SDL_Log("Subsystems Initialized");

    // create window
    // TODO: get from file
    this->window = SDL_CreateWindow("Snake Game - cpp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 740, 800, 0);
    if (!this->window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
        return;
    }
    SDL_Log("Window created");

    // create renderer
    if (!(this->renderer = SDL_CreateRenderer(this->window, -1, 0))) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
        return;
    }
    SDL_Log("Renderer created");

    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initilize fonts: %s", TTF_GetError());
        return;
    }
    SDL_Log("Fonts Initialized");
}

Context::~Context() {
    SDL_Log("Cleaning up...");

    SDL_DestroyWindow(this->window);
    SDL_Log("Window destroyed");

    SDL_DestroyRenderer(this->renderer);
    SDL_Log("Renderer destroyed");

    TTF_Quit();
    SDL_Log("Fonts Closed");

    SDL_Quit();
    SDL_Log("Clean up Done!");
}