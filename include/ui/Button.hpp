#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

class Button {
private:
    bool clicking, clicked;
    std::string name;

    SDL_Rect button;
    SDL_Surface* surface;

public:
    Button(std::string text, int x, int y, TTF_Font* font, SDL_Color color);
    ~Button();

    void toggle();
    void handleEvents(SDL_Event& event);
    void render(SDL_Renderer* renderer);

    bool isClicking() const;
    bool isClicked() const;
};