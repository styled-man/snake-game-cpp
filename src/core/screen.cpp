#include "core/screen.hpp"

// Initialize the static member variables
int Screen::instanceCount = 0;
SDL_Window* Screen::window = nullptr;
SDL_Renderer* Screen::renderer = nullptr;

Screen::Screen() {
    if (this->instanceCount == 0) {
        SDL_Log("Starting setup procedure");

        // initilize sdl
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize subsystems: %s", SDL_GetError());
            return;
        }
        SDL_Log("Subsystems Initialized");

        // create window
        // TODO: get from file
        this->window =
            SDL_CreateWindow("Snake Game - cpp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 740, 800, 0);
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
        SDL_Log("Setup procedure finished");
    } else {
        SDL_Log("Already ran setup procedure");
    }

    this->instanceCount++;
}

Screen::~Screen() {
    if (instanceCount-- > 1) {
        return;
    }

    SDL_Log("Cleaning up...");

    if (this->getFont()) {
        TTF_CloseFont(this->getFont());
        SDL_Log("Fonts Closed");
    }

    TTF_Quit();
    SDL_Log("Fonts Deinitialize");

    SDL_DestroyWindow(this->window);
    SDL_Log("Window destroyed");

    SDL_DestroyRenderer(this->renderer);
    SDL_Log("Renderer destroyed");

    SDL_Quit();
    SDL_Log("Clean up Done!");
}

SDL_Renderer* Screen::getRenderer() const {
    return this->renderer;
}

SDL_Window* Screen::getWindow() const {
    return this->window;
}

bool Screen::setFont(std::string font, int size) {
    // close old font
    if (this->getFont()) {
        TTF_CloseFont(this->getFont());
    }

    // get new font
    this->font = TTF_OpenFont(font.c_str(), size);

    // check for error
    if (!this->getFont()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", font.c_str());
        return false;
    }

    return true;
}

TTF_Font* Screen::getFont() const {
    return this->font;
}

void Screen::renderText(std::string str, int x, int y, SDL_Color color, SDL_Renderer* renderer) {
    if (!this->getFont()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No fonts set");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(this->getFont(), str.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}