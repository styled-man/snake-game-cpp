#include "screens/main_menu.hpp"

MainMenu::MainMenu() {
}

MainMenu::~MainMenu() {
}

void MainMenu::handleEvents(const SDL_Event& event) {
}

void MainMenu::update() {
}

void MainMenu::render() {
    // Main text
    int windowWidth, windowHeight, textWidth;
    SDL_GetWindowSize(this->getWindow(), &windowWidth, &windowHeight);
    this->setFont("./assets/fonts/orange juice 2.0.ttf", 100);
    TTF_SizeText(this->getFont(), "Snake Game", &textWidth, NULL);
    this->renderText("Snake Game", (windowWidth - textWidth) / 2, 80, {255, 255, 255}, this->getRenderer());
}