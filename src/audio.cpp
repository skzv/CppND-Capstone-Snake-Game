#include "SDL.h"
#include "audio.h"
#include <iostream>

#define MUS_PATH "data/Enigma-Long-Version-Complete-Version.wav"

void Audio::AudioCallback(void* userdata, Uint8* stream, int len) {

    if (audio_len == 0)
        return;

    len = (len > audio_len ? audio_len : len);
    SDL_memcpy(stream, audio_pos, len);

    audio_pos += len;
    audio_len -= len;
}

void Audio::Stop() {
    running = false;
}

void Audio::PlayMusic() {
    running = true;

    Uint32 wav_length; 
    Uint8* wav_buffer;
    SDL_AudioSpec wav_spec; 

    while (running) {

        if (SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL) {
            std::cerr << "Couldn't open audio file:  " << SDL_GetError() << "\n";
            return;
        }

        wav_spec.callback = AudioCallback;
        wav_spec.userdata = NULL;

        audio_pos = wav_buffer; // copy sound buffer
        audio_len = wav_length; // copy file length

        if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
            std::cerr << "Couldn't open audio:  " << SDL_GetError() << "\n";
            return;
        }

        SDL_PauseAudio(0);

        while (running && audio_len > 0) {
            SDL_Delay(1);
        }

        SDL_CloseAudio();
    }

    SDL_FreeWAV(wav_buffer);
}