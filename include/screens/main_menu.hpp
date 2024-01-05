#pragma once

#include "core/screen.hpp"
#include "ui/Button.hpp"

class MainMenu : public Screen {
private:
    Button* startGameButton;
    Button* settingsButton;
    Button* quitButton;

public:
    MainMenu();
    ~MainMenu();

    void handleEvents( SDL_Event& event) override;
    void update() override;
    void render() override;
};