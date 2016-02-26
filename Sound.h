#ifndef __SOUND_h_
#define __SOUND_h_

#include <SDL/SDL_mixer.h>
#include <string>

using namespace std;

class Sound {
protected:


public:
	Mix_Music* music_file;
	Mix_Chunk* effect_file;
	bool running;
	int sound_type;


	Sound(string path, int type);
	~Sound(void);

	void play(int repetition);
};

#endif