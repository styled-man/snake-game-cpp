#include "screens/game.hpp"

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

void Game::handleEvents(SDL_Event& event) {
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
    if (this->interval >= 60 / this->speed) {
        this->interval = 0;
        this->snake->move();

        std::vector<std::array<int, 2>> snakeBody = this->snake->getBody();

        // if the snake is touching it self
        for (int i = 0; i < this->snake->getLength() - 1; i++) {
            if (this->snake->getHead()[0] == snakeBody[i][0] && this->snake->getHead()[1] == snakeBody[i][1]) {
                SDL_Log("Snake collided with itself");
                Screen::state = Screen::State::MAIN_MENU;
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

void Game::render() {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(this->getWindow(), &windowWidth, &windowHeight);

    int headerHeight = windowHeight - windowWidth, padding = 30;

    // header background
    SDL_Rect header = {0, 0, windowWidth, headerHeight};
    SDL_SetRenderDrawColor(this->getRenderer(), 84, 116, 54, 255);
    SDL_RenderFillRect(this->getRenderer(), &header);

    // score Text
    this->setFont("./assets/fonts/The Sunlight.otf", 40);
    this->renderText(
        "Score: " + std::to_string(this->score), padding, headerHeight / 3, {255, 255, 255}, this->getRenderer());

    // the background color of the game
    SDL_Rect background = {0, headerHeight, windowWidth, windowHeight - headerHeight};
    SDL_SetRenderDrawColor(this->getRenderer(), 98, 137, 64, 255);
    SDL_RenderFillRect(this->getRenderer(), &background);

    int gridSizeWidth = (windowWidth - (padding * 2)) / this->gameBoardTotalCols;
    int gridSizeHeight = (windowHeight - (headerHeight + padding * 2)) / this->gameBoardTotalRows;
    int gridSize = std::min(gridSizeWidth, gridSizeHeight);

    for (int i = 0; i < this->gameBoardTotalCols; i++) {
        for (int j = 0; j < this->gameBoardTotalRows; j++) {
            // Determine the position and size for the current tile
            SDL_Rect tile = {padding + i * gridSize, headerHeight + padding + j * gridSize, gridSize, gridSize};

            // Set the color based on the game element at this position
            if (this->fruit->isColliding(i, j)) {
                // Fruit position - red
                SDL_SetRenderDrawColor(this->getRenderer(), 255, 0, 0, 255);
            } else if (this->snake->getHead()[0] == i && this->snake->getHead()[1] == j) {
                // Snake's head - dark blue
                SDL_SetRenderDrawColor(this->getRenderer(), 67, 96, 191, 255);
            } else if (this->snake->isColliding(i, j)) {
                // Snake body - lighter blue
                SDL_SetRenderDrawColor(this->getRenderer(), 81, 114, 224, 255);
            } else if ((i + j) % 2 == 0) {
                // Checkered pattern - light green
                SDL_SetRenderDrawColor(this->getRenderer(), 172, 208, 94, 255);
            } else {
                // Checkered pattern - dark green
                SDL_SetRenderDrawColor(this->getRenderer(), 178, 216, 96, 255);
            }

            // Render the tile
            SDL_RenderFillRect(this->getRenderer(), &tile);
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
