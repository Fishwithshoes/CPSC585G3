#include "Game.h"
#include "GeoGenerator.h"
#include "Loader.h"
<<<<<<< HEAD
=======
#include "Renderer.h"
>>>>>>> conlan

#include <SDL.h>

GameObject Game::skybox = GameObject();
vector<GameObject> Game::worldObjectList = {};
vector<GameObject> Game::particleObjectList = {};
vector<GameObject> Game::overlayObjectList = {};

//static Uint8 *audio_chunk;
//static Uint32 audio_len;
//static Uint8 *audio_pos;
//
//void fillAudioCallback(void *data, Uint8 *stream, int len)
//{
//	if (audio_len == 0)
//		return;
//
//	len = (len > audio_len ? audio_len : len);
//	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
//	audio_pos += len;
//	audio_len -= len;
//}

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
<<<<<<< HEAD
		load.transform.Scale(vec3(-0.9,-0.9,-0.9));
		load.standardMat.diffuseMap = MAP_SPIDER;
=======
		load.transform.Scale(vec3(-0.9, -0.9, -0.9));
		load.standardMat.diffuseMap = MAP_CHECKER;
>>>>>>> conlan
		//load.standardMat.reflectivity = 0;
		//load.standardMat.diffuseLevel = 0;
		//load.standardMat.selfIllumLevel = 1;
		Game::CreateWorldObject(load);
	}

<<<<<<< HEAD
=======
	//Skybox
	skybox.mesh = GeoGenerator::MakeSphere(1000, 4, 8, true);
	skybox.particleOverlayMat.mainTexture = MAP_ENV;

>>>>>>> conlan
	//Add initial World GameObjects
	GameObject temp = GameObject();//Change props then create with this
	GameObject *ptr;//Assign the result of creation to this and then add components

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1, 1, 2, 32);
	temp.transform.Translate(vec3(3, 2, 0));
	temp.name = "Flumpty";
	ptr = Game::CreateWorldObject(temp);

	//ifndef Car Cacophony
	temp = GameObject();
	temp.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	temp.transform.Translate(vec3(-3, 2, 0));
	ptr = Game::CreateWorldObject(temp);
	ptr->AddComponent(new VehicleComponent());
	
	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1.0, 1.0, 0.4, 16);
	temp.transform.position += vec3(-1, -0.8, 1);
	temp.transform.Rotate(vec3(0,0,Mathf::PI/2));
	temp.transform.parent = &ptr->transform;
	Game::CreateWorldObject(temp);

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1.0, 1.0, 0.4, 16);
	temp.transform.position += vec3(1, -0.8, 1);
	temp.transform.Rotate(vec3(0, 0, Mathf::PI / 2));
	temp.transform.parent = &ptr->transform;
	Game::CreateWorldObject(temp);

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1.0, 1.0, 0.4, 16);
	temp.transform.position += vec3(1, -0.8, -1);
	temp.transform.Rotate(vec3(0, 0, Mathf::PI / 2));
	temp.transform.parent = &ptr->transform;
	Game::CreateWorldObject(temp);

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1.0, 1.0, 0.4, 16);
	temp.transform.position += vec3(-1, -0.8, -1);
	temp.transform.Rotate(vec3(0, 0, Mathf::PI / 2));
	temp.transform.parent = &ptr->transform;
	Game::CreateWorldObject(temp);
	//endif Car Cacophony

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeSphere(1, 16, 32, false);
	temp.transform.Translate(vec3(0, 1, 0));
	temp.standardMat.tileUV = vec2(4,4);
	temp.standardMat._0degRef = 0.2;
	temp.standardMat.roughness = 0.3;
	temp.standardMat.diffuseColor = vec3(0.4, 0.2, 1);
	//temp.standardMat.diffuseMap = MAP_CHECKER;
	//temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	//temp.standardMat.roughnessMap = MAP_CHECKER;
	ptr = Game::CreateWorldObject(temp);
	ptr->AddComponent(new FloatComponent());
	
	temp = GameObject();
	temp.mesh = GeoGenerator::MakePlane(100, 100, 4, 4);
	temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	temp.standardMat.bumpLevel = 2;
	temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	temp.standardMat.tileUV = vec2(12,12);
	Game::CreateWorldObject(temp);
<<<<<<< HEAD
	
	/*temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1, 2, 32);
	temp.transform.Translate(vec3(3, 2, 0));
	Game::CreateWorldObject(temp);
=======
>>>>>>> conlan

	//Add initial Overlay GameObjects
	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCircle(0.8, 64, 0.25);
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

GameObject* Game::CreateWorldObject(GameObject object)
{
	object.objectID = worldObjectList.size();
	object.Start();
	worldObjectList.push_back(object);
	return &worldObjectList[worldObjectList.size() - 1];
}
GameObject* Game::CreateParticleObject(GameObject object)
{
	object.objectID = particleObjectList.size();
	object.Start();
	particleObjectList.push_back(object);
	return &particleObjectList[particleObjectList.size() - 1];
}
GameObject* Game::CreateOverlayObject(GameObject object)
{
	object.objectID = overlayObjectList.size();
	object.Start();
	overlayObjectList.push_back(object);
	return &overlayObjectList[overlayObjectList.size() - 1];
}

void Game::DestroyWorldObject(GameObject object)
{
	worldObjectList.erase(worldObjectList.begin() + object.objectID);
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (worldObjectList[i].objectID > object.objectID)
			worldObjectList[i].objectID--;
	}
	object.RemoveComponents();
}
void Game::DestroyParticleObject(GameObject object)
{
	particleObjectList.erase(particleObjectList.begin() + object.objectID);
	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (particleObjectList[i].objectID > object.objectID)
			particleObjectList[i].objectID--;
	}
	object.RemoveComponents();
}
void Game::DestroyOverlayObject(GameObject object)
{
	overlayObjectList.erase(overlayObjectList.begin() + object.objectID);
	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (overlayObjectList[i].objectID > object.objectID)
			overlayObjectList[i].objectID--;
	}
	object.RemoveComponents();
}

GameObject* Game::Find(string name)
{
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (name == worldObjectList[i].name)
			return &worldObjectList[i];
	}

	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (name == particleObjectList[i].name)
			return &particleObjectList[i];
	}

	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (name == overlayObjectList[i].name)
			return &overlayObjectList[i];
	}

	cout << "GameObject with name: " << name << " not found! Check thy spelling?" << endl;
	return nullptr;//Nothing was found
}

vector<GameObject*> Game::FindGameObjectsWithTag(Tags tag)
{
	vector<GameObject*> result;

	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (tag == worldObjectList[i].tag)
			result.push_back(&worldObjectList[i]);
	}

	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (tag == particleObjectList[i].tag)
			result.push_back(&particleObjectList[i]);
	}

	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (tag == overlayObjectList[i].tag)
			result.push_back(&overlayObjectList[i]);
	}

	return result;
}