#include "ui/button.hpp"

Button::Button(std::string text, int x, int y, TTF_Font* font, SDL_Color color) {
    this->name = text;
    this->surface = TTF_RenderText_Solid(font, text.c_str(), color);
    this->button = {x, y, surface->w, surface->h};

    // by default make each button visible
    this->isVisible = true;
}

Button::~Button() {
    SDL_FreeSurface(surface);
}

inline bool isInside(std::array<int, 2> mousePos, SDL_Rect button) {
    bool isInsideWidth = mousePos[0] > button.x && mousePos[0] < button.x + button.w;
    bool isInsideHeight = mousePos[1] > button.y && mousePos[1] < button.y + button.h;

    return isInsideWidth && isInsideHeight;
}

void Button::toggle() {
    this->clicked = !this->clicked;
}

void Button::handleEvents(SDL_Event& event) {
    if (!this->isVisible) return;

    // Get mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: {
            if (isInside({mouseX, mouseY}, this->button)) {
                this->clicking = true;
            }

            break;
        }

        case SDL_MOUSEBUTTONUP:
            // whenever they let go of the mouse anywhere
            this->clicking = false;

            // if they let go of the mouse inside of the same button, that means they clicked it
            if (isInside({mouseX, mouseY}, this->button)) {
                this->clicked = true;
                SDL_Log("`%s` button clicked", this->name.c_str());
            }
            break;

        default:
            break;
    }
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    SDL_RenderCopy(renderer, texture, NULL, &this->button);
    SDL_DestroyTexture(texture);
}

bool Button::isClicking() const {
    return this->clicking;
}

bool Button::isClicked() const {
    return this->clicked;
}