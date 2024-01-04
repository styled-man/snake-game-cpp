#pragma once

#include <SDL.h>

#include "core/context.hpp"

class Screen {
public:
    virtual void handleEvents(const SDL_Event &event) = 0;
    virtual void render(const Context &context) = 0;
    virtual void update() = 0;
};
