#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const& controller, Renderer& renderer,
    std::size_t target_frame_duration);
  void GameOver(Controller const& controller, Renderer& renderer, std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  std::vector<std::unique_ptr<Snake>> snakes;
  std::vector<SDL_Point> food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  bool IsGameOver();
  void PlaceFood();
  void PlaceFood(SDL_Point& food);
  void Update();
};

#endif