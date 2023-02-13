#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::CheckCollision(Snake& other_snake) {
  int head_x_cell = static_cast<int>(head_x);
  int head_y_cell = static_cast<int>(head_y);
  int other_head_x_cell = static_cast<int>(other_snake.head_x);
  int other_head_y_cell = static_cast<int>(other_snake.head_y);

  // Check if the head of the other snake has collided with the body of this snake.
  for (auto const &item : body) {
    if (other_head_x_cell == item.x && other_head_y_cell == item.y) {
      alive = false;
      other_snake.alive = false;
      return;
    }
  }
  // Check if the head of this snake has collided with the body of the other snake.
  for (auto const &item : other_snake.body) {
    if (head_x_cell == item.x && head_y_cell == item.y) {
      alive = false;
      other_snake.alive = false;
      return;
    }
  }
  if (head_x_cell == other_head_x_cell && head_y_cell == other_head_y_cell) {
      alive = false;
      other_snake.alive = false;
      return;
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}