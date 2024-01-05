#pragma once
#include <iostream>
#include <vector>

class Snake {
public:
    // constructor/destructor
    Snake(int maxX, int maxY);
    ~Snake();

    enum Direction { UP, DOWN, LEFT, RIGHT };

    void grow();
    void move();

    Direction getDirection() const;
    bool setDirection(Direction val);

    std::vector<std::array<int, 2>> getBody() const;
    std::array<int, 2> getHead() const;
    int getLength() const;

    bool isColliding(int xCord, int yCord) const;

private:
    Direction direction;
    int head, tail;
    int maxX, maxY;
    std::vector<std::array<int, 2>> body;
};
