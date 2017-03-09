#include "Audio.h"
#include "SDL.h"

Transform Audio::listenerTransform = Transform::identity();

vector<string> Audio::soundFilePaths =
{
	"Sounds/hit_SFX.wav",
	"Sounds/pause_SFX.wav",
	"Sounds/select_SFX.wav",
	"Sounds/machinefire_SFX.wav",
	"Sounds/powerup_SFX.wav",
};

vector<string> Audio::musicFilePaths =
{
	"Sounds/Automation.wav"
};

vector<Mix_Chunk*> Audio::sounds = {};
vector<Mix_Music*> Audio::musics = {};

int Audio::channelCount = 256;
int Audio::currentChannel = 0;

void Audio::Init()
{
	//Open SDL mixer listener
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		cout << "Flumpty can't initialize SDL listener!" << endl;
	}
	//Allocate sound channels
	int channelCount = 256;
	if (Mix_AllocateChannels(channelCount) < 0)
	{
		cout << "Flumpty can't allocate channels!" << endl;
	}
	//Check to make sure allocation worked
	if (Mix_AllocateChannels(-1) != channelCount)
	{
		cout << "SDL_mixer doesn't support that many channels!" << endl;
	}
	//Load in all sounds to program
	LoadSounds();
}

void Audio::Play2DSound(Sounds sound, float volume, float pan)
{
	Mix_Chunk* snd = sounds[sound];

	Mix_VolumeChunk(snd, (int)(volume * 128));
	int left = 255 - (int)(pan * 255);
	if (left > 255) left = 255;
	int right = 255 - (int)(-pan * 255);
	if (right > 255) right = 255;
	Mix_SetPanning(currentChannel, left, right);
	Mix_PlayChannel(currentChannel, snd, 0);

	IncrementChannel();
}

int Audio::Play2DSoundLoop(Sounds sound, float volume, float pan)
{
	Mix_Chunk* snd = sounds[sound];

	Mix_VolumeChunk(snd, (int)(volume * 128));
	int left = 255 - (int)(pan * 255);
	if (left > 255) left = 255;
	int right = 255 - (int)(-pan * 255);
	if (right > 255) right = 255;
	Mix_SetPanning(currentChannel, left, right);
	Mix_PlayChannel(currentChannel, snd, -1);

	IncrementChannel();
	return currentChannel - 1;
}

void Audio::Play3DSound(Sounds sound, vec3 sourcePoint, float volume)
{
	vec3 soundDir = sourcePoint - listenerTransform.position;
	float finalVolume = clamp((1.0f - length(soundDir)*0.01f) * volume, 0.0f, 1.0f);

	float left = 1.0 + dot(normalize(soundDir), -listenerTransform.GetRight());
	left = clamp(left, 0.0f, 1.0f);
	float right = 1.0 + dot(normalize(soundDir), listenerTransform.GetRight());
	right = clamp(right, 0.0f, 1.0f);

	Mix_Chunk* snd = sounds[sound];

	Mix_VolumeChunk(snd, (int)finalVolume * 128);
	Mix_SetPanning(currentChannel, (int)(left*255), (int)(right*255));
	Mix_PlayChannel(currentChannel, snd, 0);

	IncrementChannel();
}

int Audio::Play3DSoundLoop(Sounds sound, vec3 sourcePoint, float volume)
{
	vec3 soundDir = sourcePoint - listenerTransform.position;
	float finalVolume = clamp((1.0f - length(soundDir)*0.01f) * volume, 0.0f, 1.0f);

	float left = 1.0 + dot(normalize(soundDir), -listenerTransform.GetRight());
	left = clamp(left, 0.0f, 1.0f);
	float right = 1.0 + dot(normalize(soundDir), listenerTransform.GetRight());
	right = clamp(right, 0.0f, 1.0f);

	Mix_Chunk* snd = sounds[sound];

	Mix_VolumeChunk(snd, (int)finalVolume * 128);
	Mix_SetPanning(currentChannel, (int)(left * 255), (int)(right * 255));
	Mix_PlayChannel(currentChannel, snd, -1);

	IncrementChannel();
	return currentChannel - 1;
}

void Audio::StopSoundLoop(int channel)
{
	Mix_HaltChannel(channel);
}

void Audio::FadeSoundLoop(int channel, int milliseconds)
{
	Mix_FadeOutChannel(channel, milliseconds);
}

void Audio::StopAllSounds()
{
	Mix_HaltChannel(-1);
}

void Audio::PlayMusic(Musics music, float volume)
{
	Mix_Music* mus = musics[music];

	Mix_VolumeMusic((int)(volume * 128));
	Mix_PlayMusic(mus, -1);
}

void Audio::StopMusic()
{
	Mix_HaltMusic();
}

void Audio::FadeMusic(int milliseconds)
{
	Mix_FadeOutMusic(milliseconds);
}

void Audio::LoadSounds()
{
	for (int i = 0; i < soundFilePaths.size(); i++)
	{
		Mix_Chunk* sound = Mix_LoadWAV(soundFilePaths[i].c_str());
		if (sound == NULL)
		{
			cout << "Flumpty can't load sound at: " << soundFilePaths[i] << " !" << endl;
		}
		sounds.push_back(sound);
	}

	for (int i = 0; i < musicFilePaths.size(); i++)
	{
		Mix_Music* music = Mix_LoadMUS(musicFilePaths[i].c_str());
		if (music == NULL)
		{
			cout << "Flumpty can't load music at: " << musicFilePaths[i] << " !" << endl;
		}
		musics.push_back(music);
	}
}

void Audio::IncrementChannel()
{
	currentChannel++;
	if (currentChannel >= channelCount)
		currentChannel = 0;
}

void Audio::CleanUp()
{
	for (int i = 0; i < sounds.size(); i++)
	{
		delete sounds[i];
		sounds[i] = nullptr;
	}
	sounds.clear();
	for (int i = 0; i < musics.size(); i++)
	{
		delete musics[i];
		musics[i] = nullptr;
	}
	musics.clear();
}