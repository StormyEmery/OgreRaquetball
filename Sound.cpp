#include "Sound.h"

Sound::Sound(string path, int type){
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    sound_type = type;
    
    if(sound_type == 0){
        music_file = Mix_LoadMUS(path.c_str());
        effect_file = NULL;
    }

    else if(sound_type == 1){
        effect_file = Mix_LoadWAV(path.c_str());
        music_file = NULL;
    }
    
    running = false;
}

Sound::Sound(){}

Sound::~Sound(){
    Mix_FreeMusic(music_file);
    Mix_FreeChunk(effect_file);
    Mix_CloseAudio();
}

void Sound::play(int repetition){
    if(sound_type == 0){
        if(!running){
            Mix_PlayMusic(music_file, repetition);
            running = true;
        }
    }
    else if(sound_type == 1)
        Mix_PlayChannel(-1, effect_file, repetition);
}

void Sound::stop(){
    if(sound_type == 0)
        Mix_HaltMusic();
}

void Sound::pause(){ 
    if(sound_type == 0)
        Mix_PauseMusic(); 
}
void Sound::resume(){
    if(sound_type == 0)
        Mix_ResumeMusic();
}
