#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake& snake, Snake::Direction input,
  Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInputGameOver(bool& running) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    }
    else if (e.type == SDL_KEYDOWN) {
    }
  }
}

void Controller::HandleInput(bool& running, std::vector<std::unique_ptr<Snake>>& snakes) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    }
    else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        // snake_0
      case SDLK_w:
        ChangeDirection(*snakes[0], Snake::Direction::kUp,
          Snake::Direction::kDown);
        break;

      case SDLK_s:
        ChangeDirection(*snakes[0], Snake::Direction::kDown,
          Snake::Direction::kUp);
        break;

      case SDLK_a:
        ChangeDirection(*snakes[0], Snake::Direction::kLeft,
          Snake::Direction::kRight);
        break;

      case SDLK_d:
        ChangeDirection(*snakes[0], Snake::Direction::kRight,
          Snake::Direction::kLeft);
        break;
        // snake_1
      case SDLK_UP:
        ChangeDirection(*snakes[1], Snake::Direction::kUp,
          Snake::Direction::kDown);
        break;

      case SDLK_DOWN:
        ChangeDirection(*snakes[1], Snake::Direction::kDown,
          Snake::Direction::kUp);
        break;

      case SDLK_LEFT:
        ChangeDirection(*snakes[1], Snake::Direction::kLeft,
          Snake::Direction::kRight);
        break;

      case SDLK_RIGHT:
        ChangeDirection(*snakes[1], Snake::Direction::kRight,
          Snake::Direction::kLeft);
        break;
      }
    }
  }
}