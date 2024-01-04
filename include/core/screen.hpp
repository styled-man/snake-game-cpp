#pragma once

#include <SDL.h>

#include <iostream>

#include "core/context.hpp"

class Screen {
public:
    virtual ~Screen();
    
    virtual void handleEvents(const SDL_Event& event) = 0;
    virtual void render(const Context& context) = 0;
    virtual void update() = 0;

    TTF_Font* getFont() const;
    bool setFont(std::string font, int size);

private:
    TTF_Font* font;

protected:
    void renderText(std::string str, int x, int y, SDL_Color color, SDL_Renderer* renderer);
};
