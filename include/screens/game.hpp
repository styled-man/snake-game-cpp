#pragma once

#include "core/screen.hpp"
#include "entities/fruit.hpp"
#include "entities/snake.hpp"

#include "ui/button.hpp"

class Game : public Screen {
private:
    bool isSnakeAlive, hasStarted, isPaused;
    int speed, score;
    float interval;
    int gameBoardTotalCols, gameBoardTotalRows;

    Button* newGameButton;
    Button* mainMenuButton;

    Snake *snake;
    Fruit *fruit;

    void reset();
    void handleGameOver();
    void handlePaused();
    void renderOverlay();

public:
    // constructor/destructor
    Game();
    ~Game();

    void handleEvents(SDL_Event &event) override;
    void update() override;
    void render() override;
};