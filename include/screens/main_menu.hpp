#pragma once

#include <SDL.h>

#include <iostream>

#include "core/screen.hpp"

class MainMenu : public Screen {
private:
public:
    MainMenu();
    ~MainMenu();

    void handleEvents(const SDL_Event &event) override;
    void update() override;
    void render() override;
};