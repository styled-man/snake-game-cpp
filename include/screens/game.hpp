#pragma once

#include "core/screen.hpp"
#include "entities/fruit.hpp"
#include "entities/snake.hpp"

class Game : public Screen {
private:
    bool snakeAlive, paused;
    int speed, score;
    float interval;
    int gameBoardTotalCols, gameBoardTotalRows;

    Snake *snake;
    Fruit *fruit;

public:
    // constructor/destructor
    Game();
    ~Game();

    // getters
    bool isSnakeAlive() const;
    bool isPaused() const;

    void handleEvents(SDL_Event &event) override;
    void update() override;
    void render() override;
};