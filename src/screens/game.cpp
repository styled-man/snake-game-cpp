#include "screens/game.hpp"

#include <vector>

Game::Game() {
    SDL_Log("Starting game...");
    this->gameBoardTotalCols = this->gameBoardTotalRows = 34;

    this->snake = new Snake(this->gameBoardTotalCols, this->gameBoardTotalRows);
    this->fruit = new Fruit(this->gameBoardTotalCols, this->gameBoardTotalRows);

    this->snakeAlive = true;

    this->paused = false;
    this->speed = 9;  // TODO: get from file
    this->score = 0;
    this->interval = 0;

    SDL_Log("Game Started!");
}

Game::~Game() {
    delete this->snake;
    delete this->fruit;
}

void Game::handleEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        Snake::Direction newDirection;

        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                this->paused = !this->paused;
                SDL_Log("Game state: %s", (this->paused ? "paused" : "not paused"));
                break;

            case SDL_SCANCODE_UP:
                newDirection = Snake::Direction::UP;
                break;

            case SDL_SCANCODE_DOWN:
                newDirection = Snake::Direction::DOWN;
                break;

            case SDL_SCANCODE_LEFT:
                newDirection = Snake::Direction::LEFT;
                break;

            case SDL_SCANCODE_RIGHT:
                newDirection = Snake::Direction::RIGHT;
                break;

            default:
                break;
        }

        Snake::Direction oldDirection = this->snake->getDirection();

        if (!this->paused && this->snake->setDirection(newDirection)) {
            SDL_Log("Snake direction changed - old: %d, new: %d", oldDirection, this->snake->getDirection());
        }
    }
}

void Game::update() {
    if (this->paused) {
        return;
    }

    this->interval++;
    if (interval >= 60 / speed) {
        interval = 0;
        this->snake->move();

        std::vector<std::array<int, 2>> snakeBody = this->snake->getBody();

        // if the snake is touching it self
        for (int i = 0; i < this->snake->getLength() - 1; i++) {
            if (this->snake->getHead()[0] == snakeBody[i][0] && this->snake->getHead()[1] == snakeBody[i][1]) {
                SDL_Log("Snake collided with itself");
                this->snakeAlive = false;
                return;
            }
        }

        // if the snake is touch the fruit
        if (this->fruit->isColliding(this->snake->getHead()[0], this->snake->getHead()[1])) {
            this->score++;
            this->snake->grow();

            // ensure that the fruit does not spawn in the middle of the snake
            std::array<int, 2> fruitPos = this->fruit->getPosition();
            while (this->snake->isColliding(fruitPos[0], fruitPos[1])) {
                this->fruit->changePosition();
                fruitPos = this->fruit->getPosition();
            }

            SDL_Log("Snake eats fruit - score: %d", score);
        }
    }
}

void Game::render(const Context& context) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(context.window, &windowWidth, &windowHeight);

    int headerHeight = windowHeight - windowWidth, padding = 30;

    // header background
    SDL_Rect header = {0, 0, windowWidth, headerHeight};
    SDL_SetRenderDrawColor(context.renderer, 84, 116, 54, 255);
    SDL_RenderFillRect(context.renderer, &header);

    // score Text
    this->setFont("./assets/fonts/The Sunlight.otf", 40);
    this->renderText(
        "Score: " + std::to_string(this->score), padding, headerHeight / 3, {255, 255, 255}, context.renderer);

    // the background color of the game
    SDL_Rect background = {0, headerHeight, windowWidth, windowHeight - headerHeight};
    SDL_SetRenderDrawColor(context.renderer, 98, 137, 64, 255);
    SDL_RenderFillRect(context.renderer, &background);

    // the background color of the board
    SDL_Rect board = {padding, headerHeight + padding, windowWidth - (padding * 2), windowHeight - (headerHeight * 2)};
    SDL_SetRenderDrawColor(context.renderer, 178, 216, 96, 255);
    SDL_RenderFillRect(context.renderer, &board);

    int gridSizeWidth = (windowWidth - (padding * 2)) / this->gameBoardTotalCols;
    int gridSizeHeight = (windowHeight - (headerHeight + padding * 2)) / this->gameBoardTotalRows;
    int gridSize = std::min(gridSizeWidth, gridSizeHeight);

    // render a checked board pattern on the gameboard
    bool alternate = false;
    for (int i = 0; i < this->gameBoardTotalCols; i++) {
        alternate = !alternate;
        for (int j = 0; j < this->gameBoardTotalRows; j++) {
            alternate = !alternate;
            if (this->fruit->isColliding(i, j)) {
                SDL_Rect title = {
                    padding + i * (gridSize), headerHeight + padding + j * (gridSize), gridSize, gridSize};
                SDL_SetRenderDrawColor(context.renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(context.renderer, &title);
            }

            // color the head of the snake differently
            else if (this->snake->getHead()[0] == i && this->snake->getHead()[1] == j) {
                SDL_Rect snakeHeadSegment = {
                    padding + i * (gridSize), headerHeight + padding + j * (gridSize), gridSize, gridSize};
                SDL_SetRenderDrawColor(context.renderer, 67, 96, 191, 255);
                SDL_RenderFillRect(context.renderer, &snakeHeadSegment);
            }

            else if (this->snake->isColliding(i, j)) {
                SDL_Rect snakeSegment = {
                    padding + i * (gridSize), headerHeight + padding + j * (gridSize), gridSize, gridSize};
                SDL_SetRenderDrawColor(context.renderer, 81, 114, 224, 255);
                SDL_RenderFillRect(context.renderer, &snakeSegment);
            }

            else if (alternate) {
                SDL_Rect title = {
                    padding + i * (gridSize), headerHeight + padding + j * (gridSize), gridSize, gridSize};
                SDL_SetRenderDrawColor(context.renderer, 172, 208, 94, 255);
                SDL_RenderFillRect(context.renderer, &title);
            }
        }
    }
}

// getters/setters
bool Game::isSnakeAlive() const {
    return this->snakeAlive;
}

bool Game::isPaused() const {
    return this->paused;
}
