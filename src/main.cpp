#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <thread>
#include <chrono>
#include <mutex>
#include "SDL.h"
#include "audio.h"

int main() {
  constexpr std::size_t kFramesPerSecond{ 60 };
  constexpr std::size_t kMsPerFrame{ 1000 / kFramesPerSecond };
  constexpr std::size_t kScreenWidth{ 640 };
  constexpr std::size_t kScreenHeight{ 640 };
  constexpr std::size_t kGridWidth{ 32 };
  constexpr std::size_t kGridHeight{ 32 };

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;

  std::thread t(Audio::PlayMusic);

  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  game.GameOver(controller, renderer, kMsPerFrame);

  Audio::Stop();

  // thread barrier
  t.join();

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}