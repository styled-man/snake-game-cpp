#include "core/screen.hpp"

Screen::~Screen() {
    if (this->getFont()) {
        TTF_CloseFont(this->getFont());
    }
}

bool Screen::setFont(std::string font, int size) {
    this->font = TTF_OpenFont(font.c_str(), size);

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
}