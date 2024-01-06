#include "screens/main_menu.hpp"

MainMenu::MainMenu() {
    this->setFont("./assets/fonts/ArcadeClassic.ttf", 60);
    int windowWidth, windowHeight;
    SDL_GetWindowSize(this->getWindow(), &windowWidth, &windowHeight);

    int startGameButtonWidth;
    int startGameButtonHeight;
    TTF_SizeText(this->getFont(), "Start Game", &startGameButtonWidth, &startGameButtonHeight);
    this->startGameButton = new Button(
        "Start Game", (windowWidth - startGameButtonWidth) / 2, windowHeight / 2, this->getFont(), {255, 255, 255});

    int settingsButtonWidth, settingsButtonHeight;
    TTF_SizeText(this->getFont(), "Settings", &settingsButtonWidth, &settingsButtonHeight);
    this->settingsButton = new Button("Settings",
        (windowWidth - settingsButtonWidth) / 2,
        (windowHeight / 2) + startGameButtonHeight,
        this->getFont(),
        {255, 255, 255});

    int quitButtonWidth, quitButtonHeight;
    TTF_SizeText(this->getFont(), "Quit", &quitButtonWidth, &quitButtonHeight);
    this->quitButton = new Button("Quit",
        (windowWidth - quitButtonWidth) / 2,
        (windowHeight / 2) + startGameButtonHeight + settingsButtonHeight,
        this->getFont(),
        {255, 255, 255});
}

MainMenu::~MainMenu() {
    delete this->startGameButton;
    delete this->settingsButton;
    delete this->quitButton;
}

void MainMenu::handleEvents(SDL_Event& event) {
    this->startGameButton->handleEvents(event);
    this->settingsButton->handleEvents(event);
    this->quitButton->handleEvents(event);
}

void MainMenu::update() {
    if (this->startGameButton->isClicked()) {
        Screen::state = Screen::State::IN_GAME;
        this->startGameButton->toggle();
    } 
    
    else if (this->settingsButton->isClicked()) {
        Screen::state = Screen::State::SETTINGS;
        this->settingsButton->toggle();
    } 
    
    else if (this->quitButton->isClicked()) {
        Screen::state = Screen::State::EXIT;
        this->quitButton->toggle();
    }
}

void MainMenu::render() {
    int windowWidth, windowHeight, textWidth;
    SDL_GetWindowSize(this->getWindow(), &windowWidth, &windowHeight);
    this->setFont("./assets/fonts/orange juice 2.0.ttf", 100);

    // Main text
    TTF_SizeText(this->getFont(), "Snake Game", &textWidth, NULL);
    this->renderText(
        "Snake Game", (windowWidth - textWidth) / 2, windowHeight / 2 - 100, {255, 255, 255});

    // buttons
    this->startGameButton->render(this->getRenderer());
    this->settingsButton->render(this->getRenderer());
    this->quitButton->render(this->getRenderer());
}