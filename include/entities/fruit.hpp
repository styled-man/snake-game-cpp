#pragma once
#include <iostream>

class Fruit {
private:
    int x, y, maxX, maxY;

public:
    Fruit(int maxX, int maxY);
    ~Fruit();

    std::array<int, 2> getPosition() const;
    void changePosition();
    bool isColliding(int xCord, int yCord) const;
};