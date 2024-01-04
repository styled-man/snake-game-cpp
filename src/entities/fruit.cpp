#include "entities/fruit.hpp"

Fruit::Fruit(int maxX, int maxY) {
    this->maxX = maxX;
    this->maxY = maxY;
    changePosition();
}

Fruit::~Fruit() {
}

void Fruit::changePosition() {
    this->x = rand() % this->maxX;
    this->y = rand() % this->maxY;
}

bool Fruit::isColliding(int xCord, int yCord) const {
    return this->x == xCord && this->y == yCord;
}

std::array<int, 2> Fruit::getPosition() const {
    return {this->x, this->y};
}