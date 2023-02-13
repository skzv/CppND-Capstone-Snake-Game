#ifndef AUDIO_H
#define AUDIO_H

#include "SDL.h"

static Uint8* audio_pos = 0; 
static Uint32 audio_len = 0;
static bool running = false;

class Audio {
 public:
  static void PlayMusic();
  static void Stop();

 private:
  static void AudioCallback(void* userdata, Uint8* stream, int len);
};

#endif