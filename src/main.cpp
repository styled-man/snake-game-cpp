#include <iostream>
#include <screens/game.hpp>

#include "core/context.hpp"

void fpsLimiter(int maxFps, Uint32 startTick) {
    if (1000 / maxFps > SDL_GetTicks() - startTick) {
        SDL_Delay(1000 / maxFps - (SDL_GetTicks() - startTick));
    }
}

int main(int argc, char* argv[]) {
    enum GameState {
        IN_GAME,
        MAIN_MENU,
        SETTINGS
    };

    Context* context = new Context();
    Game* game = new Game();

    const int MAX_FPS = 60;  // TODO: get from file

    Screen* currentScreen = game;

    SDL_Event event;
    bool isRunning = true;

    while (isRunning) {
        Uint32 startTick = SDL_GetTicks();

        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                break;
            }

            // Pass the event to the current screen for handling
            currentScreen->handleEvents(event);
        }

        // screen logic
        currentScreen->update();

        // render screen
        SDL_SetRenderDrawColor((*context).renderer, 0, 0, 0, 255);
        SDL_RenderClear((*context).renderer);
        currentScreen->render(*context);
        SDL_RenderPresent((*context).renderer);

        // cap the frame rate
        fpsLimiter(MAX_FPS, startTick);
    }

    delete context;
    delete game;

    return EXIT_SUCCESS;
}