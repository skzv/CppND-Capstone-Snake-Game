#include "renderer.h"
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>

Renderer::Renderer(const std::size_t screen_width,
  const std::size_t screen_height,
  const std::size_t grid_width, const std::size_t grid_height)
  : screen_width(screen_width),
  screen_height(screen_height),
  grid_width(grid_width),
  grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
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

  // this opens a font style and sets a size
  TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

  // this is the color in rgb format,
  // maxing out all would give you the color white,
  // and it will be your text's color
  SDL_Color White = { 255, 255, 255 };

  // as TTF_RenderText_Solid could only be used on
  // SDL_Surface then you have to create the surface first
  SDL_Surface* surfaceMessage =
    TTF_RenderText_Solid(Sans, "put your text here", White);

  // now you can convert it into a texture
  SDL_Texture* Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);

  SDL_Rect Message_rect; //create a rect
  Message_rect.x = 0;  //controls the rect's x coordinate 
  Message_rect.y = 0; // controls the rect's y coordinte
  Message_rect.w = 100; // controls the width of the rect
  Message_rect.h = 100; // controls the height of the rect

  // (0,0) is on the top left of the window/screen,
  // think a rect as the text's box,
  // that way it would be very simple to understand

  // Now since it's a texture, you have to put RenderCopy
  // in your game loop area, the area where the whole code executes

  // you put the renderer's name first, the Message,
  // the crop size (you can ignore this if you don't want
  // to dabble with cropping), and the rect which is the size
  // and coordinate of your texture
  SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);

  // Don't forget to free your surface and texture
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
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
