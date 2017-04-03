#pragma once
#include "CommonIncludes.h"
#include "SDL_mixer.h"
#include "Transform.h"

class Audio
{
public:
	static void Init();//Call at start
	static void CleanUp();//Call during cleanup phase

	//Plays 2D sound. Volume 0-1. Pan -1 - 1 Left-Right
	static void Play2DSound(Sounds sound, float volume, float pan);
	static int Play2DSoundLoop(Sounds sound, float volume, float pan);
	static void Play3DSound(Sounds sound, vec3 sourcePoint, float volume);
	static int Play3DSoundLoop(Sounds sound, vec3 sourcePoint, float volume);
	static void StopSoundLoop(int channel);
	static void FadeSoundLoop(int channel, int milliseconds);
	static void StopAllSounds();
	//Plays music track on endless loop
	static void PlayMusic(Musics music, float volume);
	static void StopMusic();
	static void FadeMusic(int milliseconds);

	static Transform listenerTransform;
protected:
private:
	static vector<string> soundFilePaths;
	static vector<string> musicFilePaths;
	static vector<Mix_Chunk*> sounds;
	static vector<Mix_Music*> musics;
	static void LoadSounds();

	static int channelCount;
	static int currentChannel;
	static void IncrementChannel();
};

