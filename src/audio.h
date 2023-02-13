#ifndef AUDIO_H
#define AUDIO_H

#include "SDL.h"

class Audio {
 public:
  static void PlayMusic();
  static void Stop();

 private:
  static void AudioCallback(void* userdata, Uint8* stream, int len);
  inline static Uint8* audio_pos = 0; 
  inline static Uint32 audio_len = 0;
  inline static bool running = false;
};

#endif