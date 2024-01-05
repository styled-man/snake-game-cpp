#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

class Screen {
private:
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static int instanceCount;

    TTF_Font* font;

protected:
    void renderText(std::string str, int x, int y, SDL_Color color, SDL_Renderer* renderer);

public:
    Screen();
    virtual ~Screen();

    virtual void handleEvents(const SDL_Event& event) = 0;
    virtual void render() = 0;
    virtual void update() = 0;

    TTF_Font* getFont() const;
    bool setFont(std::string font, int size);

    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;
};
