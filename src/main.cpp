#include <iostream>
#include <screens/game.hpp>
#include <screens/main_menu.hpp>

void fpsLimiter(int maxFps, Uint32 startTick) {
    if (1000 / maxFps > SDL_GetTicks() - startTick) {
        SDL_Delay(1000 / maxFps - (SDL_GetTicks() - startTick));
    }
}

int main(int argc, char* argv[]) {
    const int MAX_FPS = 60;  // TODO: get from file

    // screens
    Game* game = new Game();
    MainMenu* mainMenu = new MainMenu();

    // manage which screen to show
    enum GameState { IN_GAME, MAIN_MENU, SETTINGS };
    GameState currentState = GameState::IN_GAME;
    Screen* currentScreen = game;

    SDL_Event event;
    bool isRunning = true;

    while (isRunning) {
        Uint32 startTick = SDL_GetTicks();

        // show the right screen based on the state
        switch (currentState) {
            case IN_GAME:
                if (game->isSnakeAlive()) {
                    currentScreen = game;
                } else {
                    currentState = GameState::MAIN_MENU;
                    currentScreen = mainMenu;
                }
                break;
            case MAIN_MENU:
                currentScreen = mainMenu;
                break;
            case SETTINGS:
                // currentScreen = settingsScreen;
                break;
        }

        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Log("Close game");
                isRunning = false;
                break;
            }

            // Pass the event to the current screen for handling
            currentScreen->handleEvents(event);
        }

        // screen logic
        currentScreen->update();

        // render screen
        SDL_SetRenderDrawColor(currentScreen->getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(currentScreen->getRenderer());
        currentScreen->render();
        SDL_RenderPresent(currentScreen->getRenderer());

        // cap the frame rate
        fpsLimiter(MAX_FPS, startTick);
    }

    delete game;
    delete mainMenu;

    return EXIT_SUCCESS;
}