#include "entities/snake.hpp"

Snake::Snake(int maxX, int maxY) {
    srand(time(NULL));  // change the seed of rand func

    this->maxX = maxX;
    this->maxY = maxY;

    this->body.resize(this->maxX * this->maxY, {0, 0});
    this->tail = this->head = 0;

    // randomize the initial direction
    this->direction = static_cast<Snake::Direction>(rand() % 4);

    // spawn the snake in a random location
    this->body[this->head] = {rand() % this->maxX, rand() % this->maxY};

    // the initial lenght of the snake
    this->grow();
}

Snake::~Snake() {
}

void Snake::grow() {
    int oldTail = this->tail;
    move();
    this->tail = oldTail;
}

void Snake::move() {
    bool isOffscreen = false;

    int xCord = this->body[this->head][0];
    int yCord = this->body[this->head][1];

    switch (this->direction) {
        case Snake::Direction::UP:
            if (yCord-- == 0) {
                isOffscreen = true;
                yCord = this->maxY - 1;
            }
            break;

        case Snake::Direction::DOWN:
            if (yCord++ == this->maxY - 1) {
                isOffscreen = true;
                yCord = 0;
            }
            break;

        case Snake::Direction::LEFT:
            if (xCord-- == 0) {
                isOffscreen = true;
                xCord = this->maxX - 1;
            }
            break;

        case Snake::Direction::RIGHT:
            if (xCord++ == this->maxX - 1) {
                isOffscreen = true;
                xCord = 0;
            }
            break;
    }

    this->tail = (this->tail + 1) % this->body.size();
    this->head = (this->head + 1) % this->body.size();

    this->body[this->head] = {xCord, yCord};
}

Snake::Direction Snake::getDirection() const {
    return this->direction;
}

bool Snake::setDirection(Snake::Direction val) {
    Snake::Direction currentDirection = this->getDirection();

    if (currentDirection != val &&
        !((currentDirection == Snake::Direction::UP && val == Snake::Direction::DOWN) ||
            (currentDirection == Snake::Direction::DOWN && val == Snake::Direction::UP) ||
            (currentDirection == Snake::Direction::LEFT && val == Snake::Direction::RIGHT) ||
            (currentDirection == Snake::Direction::RIGHT && val == Snake::Direction::LEFT))) {
        // the new direction is different from the current direction
        // it is also not directly opposite olf the current direction
        this->direction = val;
        return true;
    }

    return false;
}

int Snake::getLength() const {
    if (this->head >= this->tail) {
        return this->head - this->tail;
    } else {
        return (body.size() - this->tail) + this->head;
    }
}

std::vector<std::array<int, 2>> Snake::getBody() const {
    std::vector<std::array<int, 2>> segments;
    segments.reserve(this->getLength());

    int i = this->tail;
    int length = this->getLength();
    while (length-- > 0) {
        segments.push_back(this->body[i]);
        i = (i + 1) % this->body.size();
    }

    return segments;
}

std::array<int, 2> Snake::getHead() const {
    return this->body[head];
}

bool Snake::isColliding(int xCord, int yCord) const {
    int i = this->tail;
    int length = this->getLength();
    while (length-- > 0) {
        if (xCord == this->body[i][0] && yCord == this->body[i][1]) {
            return true;
        }
        i = (i + 1) % this->body.size();
    }

    return false;
}
