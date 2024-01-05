#include <iostream>
#include <screens/game.hpp>
#include <screens/main_menu.hpp>

void fpsLimiter(int maxFps, Uint32 startTick) {
    if (1000 / maxFps > SDL_GetTicks() - startTick) {
        SDL_Delay(1000 / maxFps - (SDL_GetTicks() - startTick));
    }
}

// initial screen to show
Screen::State Screen::state = Screen::State::MAIN_MENU;

int main(int argc, char* argv[]) {
    const int MAX_FPS = 60;  // TODO: get from file

    // screens
    Screen* screen = nullptr;
    Game* game = new Game();
    MainMenu* mainMenu = new MainMenu();

    SDL_Event event;

    while (Screen::state != Screen::State::EXIT) {
        Uint32 startTick = SDL_GetTicks();

        // show the right screen based on the state
        switch (Screen::state) {
            case Screen::State::IN_GAME:
                screen = game;
                break;
            case Screen::State::MAIN_MENU:
                screen = mainMenu;
                break;
            case Screen::State::SETTINGS:
                // currentScreen = settingsScreen;
                break;
            default:
                break;
        }

        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Log("Close game");
                screen->state = Screen::State::EXIT;
                break;
            }

            // Pass the event to the current screen for handling
            screen->handleEvents(event);
        }

        // screen logic
        screen->update();

        // render screen
        SDL_SetRenderDrawColor(screen->getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(screen->getRenderer());
        screen->render();
        SDL_RenderPresent(screen->getRenderer());

        // cap the frame rate
        fpsLimiter(MAX_FPS, startTick);
    }

    delete game;
    delete mainMenu;

    return EXIT_SUCCESS;
}