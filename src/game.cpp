#include "game.h"
#include <iostream>
#include "SDL.h"
#include <memory>
#include <chrono>
#include <thread>

Game::Game(std::size_t grid_width, std::size_t grid_height)
  : engine(dev()),
  random_w(0, static_cast<int>(grid_width - 1)),
  random_h(0, static_cast<int>(grid_height - 1)) {

  std::unique_ptr<Snake> snake_0 = std::make_unique<Snake>(grid_width, grid_height, grid_width / 3, grid_height / 2);
  std::unique_ptr<Snake> snake_1 = std::make_unique<Snake>(grid_width, grid_height, 2 * grid_width / 3, grid_height / 2);

  snakes.emplace_back(std::move(snake_0));
  snakes.emplace_back(std::move(snake_1));

  food.emplace_back(SDL_Point());
  food.emplace_back(SDL_Point());
  PlaceFood(food);
}

void Game::GameOver(Controller const& controller, Renderer& renderer,
  std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInputGameOver(running);
    Update();
    renderer.RenderGameOver();

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(GetSize(), frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::Run(Controller const& controller, Renderer& renderer,
  std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  Uint32 game_over_start;
  int frame_count = 0;
  bool running = true;
  bool isGameOver = false;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snakes);
    Update();
    renderer.Render(snakes, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(GetSize(), frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

    if (IsGameOver()) {
      if (!isGameOver) {
        game_over_start = SDL_GetTicks();
        isGameOver = true;
      }
      // wait 1s before showing Game Over
      if(SDL_GetTicks() - game_over_start >= 1000) {
        running = false;
      }
    }
  }
}

bool Game::IsGameOver() {
  return !snakes[0]->alive && !snakes[1]->alive;
}

void Game::PlaceFood() {
  for (auto& food_item : food) {
    PlaceFood(food_item);
  }
}

void Game::PlaceFood(SDL_Point& food) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!(snakes[0]->SnakeCell(x, y)) && !(snakes[1]->SnakeCell(x, y))) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  snakes[0]->CheckCollision(*snakes[1]);

  for (auto& snake : snakes) {
    if (!snake->alive)
      continue;

    snake->Update();

    int new_x = static_cast<int>(snake->head_x);
    int new_y = static_cast<int>(snake->head_y);

    // Check if there's food over here
    for (auto& single_food : food) {
      if (single_food.x == new_x && single_food.y == new_y) {
        PlaceFood(single_food);
        // Grow snake and increase speed.
        snake->GrowBody();
        snake->speed += 0.02;
      }
    }
  }
}

int Game::GetScore() const { return GetSize(); }
int Game::GetSize() const {
  return std::min(snakes[0]->size, snakes[1]->size);
}