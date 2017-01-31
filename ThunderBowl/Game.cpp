#include "Game.h"
#include "GeoGenerator.h"
#include "Loader.h"

#include <SDL.h>

vector<GameObject> Game::worldObjectList = {};
vector<GameObject> Game::particleObjectList = {};
vector<GameObject> Game::overlayObjectList = {};

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

void fillAudioCallback(void *data, Uint8 *stream, int len)
{
	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

void Game::BuildWorld()
{
	//Pre-allocate space in vectors
	worldObjectList.reserve(1000);
	particleObjectList.reserve(100);
	overlayObjectList.reserve(50);

	//Attempt to load .obj file
	Loader gameLoader = Loader();	//check constructor
	gameLoader.loadModel("U:/CPSC585/CPSC585G3/middleware/assimp-3.1.1-win-binaries/assimp-3.1.1-win-binaries/test/models/OBJ/spider.obj");
	for (int i = 0; i < gameLoader.getMeshes().size(); i++) {
		GameObject load = GameObject();
		load.mesh = gameLoader.getMeshes()[i];
		load.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
		load.standardMat.reflectivity = 0;
		load.standardMat.diffuseLevel = 0;
		load.standardMat.selfIllumLevel = 1;
		Game::CreateWorldObject(load);
	}

	//Add initial World GameObjects
	GameObject temp = GameObject();
	temp.mesh = GeoGenerator::MakeSphere(5000, 32, 64, true);
	temp.standardMat.diffuseMap = MAP_ENV;
	temp.standardMat.reflectivity = 0;
	temp.standardMat.diffuseLevel = 0;
	temp.standardMat.selfIllumLevel = 1;
	Game::CreateWorldObject(temp);

	/*temp = GameObject();
	temp.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	temp.transform.Translate(vec3(-3, 3, 0));
	temp.standardMat.reflectivity = 0;
	Game::CreateWorldObject(temp);

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeSphere(1, 16, 32, false);
	temp.transform.Translate(vec3(0, 1, 0));
	temp.standardMat.tileUV = vec2(4,4);
	temp.standardMat.diffuseMap = MAP_CHECKER;
	temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	Game::CreateWorldObject(temp);*/
	
	temp = GameObject();
	temp.mesh = GeoGenerator::MakePlane(100, 100, 4, 4);
	temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	temp.standardMat.bumpLevel = 2;
	temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	temp.standardMat.tileUV = vec2(12,12);
	Game::CreateWorldObject(temp);
	
	/*temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1, 2, 32);
	temp.transform.Translate(vec3(3, 2, 0));
	Game::CreateWorldObject(temp);

	//Add initial Overlay GameObjects
	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCircle(1, 64, 0.25);
	temp.transform.Translate(vec3(0, 0.75, 0));
	temp.particleOverlayMat.mainTexture = MAP_JERRY;
	Game::CreateOverlayObject(temp);*/

	//SDL EAMPLE FOR AUDIO CLASS
	//Initialization
	//if (SDL_Init(SDL_INIT_AUDIO) == -1)
	//	cout << "Flumpty can't initialize SDL!" << endl;
	//
	//SDL_AudioSpec desired;
	//
	//desired.freq = 22050;
	//desired.format = AUDIO_S16SYS;
	//desired.channels = 2;
	//desired.samples = 1024;
	//desired.callback = fillAudioCallback;
	//desired.userdata = NULL;
	//
	//if(SDL_OpenAudio(&desired, NULL))
	//	cout << "Flumpty can't open audio device!" << endl;
	//
	////Load file
	//SDL_AudioSpec spec;
	//Uint32 length;
	//Uint8* buffer;
	//
	//if (SDL_LoadWAV("Sounds/jerry_MUS.wav", &spec, &buffer, &length) == NULL)
	//	cout << "Flumpty can't load thy WAV!" << endl;
	//
	//audio_pos = audio_chunk;
	//
	////Play
	//SDL_PauseAudio(0);
}

void Game::DestroyWorld()
{

}

void Game::CreateWorldObject(GameObject object)
{
	object.objectID = worldObjectList.size();
	object.Start();
	worldObjectList.push_back(object);
}
void Game::CreateParticleObject(GameObject object)
{
	object.objectID = particleObjectList.size();
	object.Start();
	particleObjectList.push_back(object);
}
void Game::CreateOverlayObject(GameObject object)
{
	object.objectID = overlayObjectList.size();
	object.Start();
	overlayObjectList.push_back(object);
}

void Game::DestroyWorldObject(GameObject object)
{
	worldObjectList.erase(worldObjectList.begin() + object.objectID);
	cout << object.objectID;
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (worldObjectList[i].objectID > object.objectID)
			worldObjectList[i].objectID--;
	}
}
void Game::DestroyParticleObject(GameObject object)
{
	particleObjectList.erase(particleObjectList.begin() + object.objectID);
	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (particleObjectList[i].objectID > object.objectID)
			particleObjectList[i].objectID--;
	}
}
void Game::DestroyOverlayObject(GameObject object)
{
	overlayObjectList.erase(overlayObjectList.begin() + object.objectID);
	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (overlayObjectList[i].objectID > object.objectID)
			overlayObjectList[i].objectID--;
	}
}