#include "screens/game.hpp"

Game::Game() {
    SDL_Log("Initializing game...");

    this->gameBoardTotalCols = this->gameBoardTotalRows = 34;
    this->reset();

    int windowWidth, windowHeight;
    int textWidth, textHeight;
    SDL_GetWindowSize(this->getWindow(), &windowWidth, &windowHeight);

    std::string text = "New Game";
    this->setFont("./assets/fonts/ArcadeClassic.ttf", 40);
    TTF_SizeText(this->getFont(), text.c_str(), &textWidth, &textHeight);
    this->newGameButton = new Button(
        text, (windowWidth - textWidth) / 2, (windowHeight - textHeight + 400) / 2, this->getFont(), {255, 255, 255});

    text = "Main Menu";
    TTF_SizeText(this->getFont(), text.c_str(), &textWidth, &textHeight);
    this->mainMenuButton = new Button(
        text, (windowWidth - textWidth) / 2, (windowHeight - textHeight + 500) / 2, this->getFont(), {255, 255, 255});

    SDL_Log("Game Initialized!");
}

void Game::reset() {
    if (this->snake) delete this->snake;
    if (this->fruit) delete this->fruit;

    this->snake = new Snake(this->gameBoardTotalCols, this->gameBoardTotalRows);
    this->fruit = new Fruit(this->gameBoardTotalCols, this->gameBoardTotalRows);

    this->isSnakeAlive = true;
    this->hasStarted = false;
    this->isPaused = false;

    this->speed = 12;  // TODO: get from file
    this->score = 0;
    this->interval = 0;
}

Game::~Game() {
    delete this->snake;
    delete this->fruit;
}

void Game::handleEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        Snake::Direction newDirection = Snake::Direction::NONE;

        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                if (this->isSnakeAlive && this->hasStarted) {
                    this->isPaused = !this->isPaused;
                    SDL_Log("Game state: %s", (this->isPaused ? "paused" : "not paused"));
                }
                return;

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
                return;
        }

        Snake::Direction oldDirection = this->snake->getDirection();

        if (!this->hasStarted && this->isSnakeAlive) {
            // start the game whenever one of the arrow keys is pressed
            this->hasStarted = true;
        }

        // if the game is not paused and the user changes the direction of the snake
        if (!this->isPaused && this->snake->setDirection(newDirection)) {
            SDL_Log("Snake direction changed - old: %d, new: %d", oldDirection, this->snake->getDirection());
        }
    }

    this->mainMenuButton->handleEvents(event);
    this->newGameButton->handleEvents(event);
}

void Game::update() {
    this->newGameButton->isVisible = false;
    this->mainMenuButton->isVisible = false;

    // new game and main menu button logic
    if (this->hasStarted && (this->isPaused || !this->isSnakeAlive)) {
        this->newGameButton->isVisible = true;
        this->mainMenuButton->isVisible = true;

        if (this->newGameButton->isClicked()) {
            this->newGameButton->toggle();
            this->reset();
            this->hasStarted = true;
        }

        else if (this->mainMenuButton->isClicked()) {
            this->mainMenuButton->toggle();
            this->isPaused = false;
            Game::state = Game::State::MAIN_MENU;
        }
    }

    else if (this->hasStarted && !this->isPaused && this->isSnakeAlive) {
        this->interval++;
        if (this->interval >= 60 / this->speed) {
            this->interval = 0;
            this->snake->move();

            std::vector<std::array<int, 2>> snakeBody = this->snake->getBody();

            // if the snake is touching it self
            for (int i = 0; i < this->snake->getLength() - 1; i++) {
                if (this->snake->getHead()[0] == snakeBody[i][0] && this->snake->getHead()[1] == snakeBody[i][1]) {
                    SDL_Log("Game Over - Snake collided with itself");
                    this->isSnakeAlive = false;
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
}

void Game::renderOverlay() {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(this->getWindow(), &windowWidth, &windowHeight);

    SDL_SetRenderDrawColor(this->getRenderer(), 0, 0, 0, 150);
    SDL_SetRenderDrawBlendMode(this->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
    SDL_RenderFillRect(this->getRenderer(), &overlay);
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
    this->renderText("Score: " + std::to_string(this->score), padding, headerHeight / 3, {255, 255, 255});

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

    // game did not start
    if (!this->hasStarted && this->isSnakeAlive) {
        this->renderOverlay();
        int textWidth, textHeight;

        std::string text = "Press an arrow key to start";
        TTF_SizeText(this->getFont(), text.c_str(), &textWidth, &textHeight);
        this->renderText(text, (windowWidth - textWidth) / 2, (windowHeight - textHeight) / 2, {255, 255, 255});
    }

    // game is paused
    else if (this->isPaused) {
        this->renderOverlay();
        int textWidth, textHeight;

        std::string text = "Press an arrow key to resume";
        TTF_SizeText(this->getFont(), text.c_str(), &textWidth, &textHeight);
        this->renderText(text, (windowWidth - textWidth) / 2, (windowHeight - textHeight) / 2, {255, 255, 255});

        text = "Paused";
        this->setFont("./assets/fonts/ArcadeClassic.ttf", 100);
        TTF_SizeText(this->getFont(), text.c_str(), &textWidth, &textHeight);
        this->renderText(text, (windowWidth - textWidth) / 2, (windowHeight - textHeight - 100) / 2, {255, 255, 255});

        this->newGameButton->render(this->getRenderer());
        this->mainMenuButton->render(this->getRenderer());
    }

    // gameover
    else if (this->hasStarted && !this->isSnakeAlive) {
        this->renderOverlay();
        int textWidth, textHeight;

        std::string text = "GameOver";
        this->setFont("./assets/fonts/ArcadeClassic.ttf", 100);
        TTF_SizeText(this->getFont(), text.c_str(), &textWidth, &textHeight);
        this->renderText(text, (windowWidth - textWidth) / 2, (windowHeight - textHeight - 100) / 2, {255, 255, 255});

        this->newGameButton->render(this->getRenderer());
        this->mainMenuButton->render(this->getRenderer());
    }
}
