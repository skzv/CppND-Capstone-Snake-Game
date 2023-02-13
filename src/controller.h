#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <memory>

class Controller {
public:
    void HandleInput(bool& running, std::vector<std::unique_ptr<Snake>>& snakes) const;
    void HandleInputGameOver(bool& running) const;

private:
    void ChangeDirection(Snake& snake, Snake::Direction input,
        Snake::Direction opposite) const;
};

#endif