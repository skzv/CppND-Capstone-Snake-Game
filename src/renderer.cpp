#include "renderer.h"
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <fstream>

Renderer::Renderer(const std::size_t screen_width,
  const std::size_t screen_height,
  const std::size_t grid_width, const std::size_t grid_height)
  : screen_width(screen_width),
  screen_height(screen_height),
  grid_width(grid_width),
  grid_height(grid_height),
  font_color({ 255, 255, 255 }) {

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  if (TTF_Init() < 0) {
    std::cerr << "TTF could not initialize.\n";
    std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
  }

  //Load font
  font = TTF_OpenFont("data/Debrosee-ALPnL.ttf", 24);
  if (font == NULL) {
    std::cerr << " SDL_Error: " << TTF_GetError() << "\n";
  }
  
  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, screen_width,
    screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::RenderGameOver() {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  SDL_Surface* surfaceMessage =
    TTF_RenderText_Solid(font, "GAME OVER", font_color);

  SDL_Texture* Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);

  SDL_Rect Message_rect;
  Message_rect.x = 0;
  Message_rect.y = 0;
  Message_rect.w = 600; 
  Message_rect.h = 100; 

  SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);
  SDL_RenderPresent(sdl_renderer);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}

void Renderer::Render(std::vector<std::unique_ptr<Snake>> const& snakes, std::vector<SDL_Point> const& food) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  SDL_Rect block; // reuse the block object for all rendering
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  for (const auto& snake : snakes) {
    // Render snake's body
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const& point : snake->body) {
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(snake->head_x) * block.w;
    block.y = static_cast<int>(snake->head_y) * block.h;
    if (snake->alive) {
      SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    }
    else {
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render food
  for (auto const& food_item : food) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food_item.x * block.w;
    block.y = food_item.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{ "Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps) };
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
