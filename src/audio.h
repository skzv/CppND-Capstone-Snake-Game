#ifndef AUDIO_H
#define AUDIO_H

#include "SDL.h"

class Audio {
 public:
  static void PlayMusic();
  static void Stop();

 private:
  static void AudioCallback(void* userdata, Uint8* stream, int len);
  static Uint8* audio_pos; 
  static Uint32 audio_len;
  static bool running;
};

#endif