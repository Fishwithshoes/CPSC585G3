#include "GameObject.h"
#include "CommonIncludes.h"
#include "Game.h"

//Only use this Constructor for objects you don't really care about...
GameObject::GameObject()
{
	transform = Transform::identity();

	//Initialize Standard Shader Props (Used to draw World Objects)
	standardMat.diffuseLevel = 1.0;
	standardMat.diffuseColor = vec3(1);
	standardMat.reflectivity = 1.0;
	standardMat.reflectColor = vec3(1);
	standardMat.roughness = 0.5;
	standardMat.curveShape = 3.0;
	standardMat._0degRef = 0.2;
	standardMat._90degRef = 1.0;
	standardMat.bumpLevel = 1.0;
	standardMat.selfIllumLevel = 0.0;
	standardMat.selfIllumColor = vec3(1);
	standardMat.rimLevel = 0.0;
	standardMat.rimColor = vec3(1);
	standardMat.rimPower = 2.0;
	standardMat.tileUV = vec2(1, 1);
	standardMat.offsetUV = vec2(0, 0);

	standardMat.diffuseMap = MAP_WHITE;
	standardMat.normalMap = MAP_NORMAL;
	standardMat.envMap = MAP_ENV;
	standardMat.roughnessMap = MAP_WHITE;

	//Initialize ParticleOverlay Shader Props (Used to draw Particles, Overlay and skybox)
	particleOverlayMat.color = vec4(1, 1, 1, 1);
	particleOverlayMat.mainTexture = MAP_WHITE;

	name = "Default";
	tag = Tags::TAGS_DEFAULT;
}

//Use this Constructor for more "Important" objects that you may want access to later (most)
GameObject::GameObject(Mesh meshIn, string nameIn, Tags tagIn)
{
	transform = Transform::identity();

	//Initialize Standard Shader Props (Used to draw World Objects)
	standardMat.diffuseLevel = 1.0;
	standardMat.diffuseColor = vec3(1);
	standardMat.reflectivity = 1.0;
	standardMat.reflectColor = vec3(1);
	standardMat.roughness = 0.5;
	standardMat.curveShape = 3.0;
	standardMat._0degRef = 0.2;
	standardMat._90degRef = 1.0;
	standardMat.bumpLevel = 1.0;
	standardMat.selfIllumLevel = 0.0;
	standardMat.selfIllumColor = vec3(1);
	standardMat.rimLevel = 0.0;
	standardMat.rimColor = vec3(1);
	standardMat.rimPower = 2.0;
	standardMat.tileUV = vec2(1, 1);
	standardMat.offsetUV = vec2(0, 0);

	standardMat.diffuseMap = MAP_WHITE;
	standardMat.normalMap = MAP_NORMAL;
	standardMat.envMap = MAP_ENV;
	standardMat.roughnessMap = MAP_WHITE;

	//Initialize ParticleOverlay Shader Props (Used to draw Particles, Overlay and skybox)
	particleOverlayMat.color = vec4(1, 1, 1, 1);
	particleOverlayMat.mainTexture = MAP_WHITE;

	mesh = meshIn;
	name = nameIn;
	tag = tagIn;
}

GameObject::~GameObject()
{
	
}

//Use this for initialization
void GameObject::Start()
{
	
}

//Use this for behaviour
void GameObject::Update()
{
	for (int i = 0; i < componentList.size(); i++)
		componentList[i]->Update();
}

mat4 GameObject::GetModelToWorld()
{
	if (transform.parent == nullptr)
	{
		return transform.GetModelToWorld();
	}
	else
	{
		return transform.GetModelToWorld() * transform.parent->GetModelToWorld();
	}
}

mat4 GameObject::GetNormalToWorld()
{
	mat4 start(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	if (transform.parent == nullptr)
	{
		return transform.GetNormalToWorld() * start;
	}
	else
	{
		return transform.GetNormalToWorld() * transform.parent->GetNormalToWorld();
	}
}

Component* GameObject::GetComponent(Component* type)
{
	//Search for component of supplied type and return the first found instance
	for (int i = 0; i < componentList.size(); i++)
	{
		if (typeid(*componentList[i]) == typeid(*type))
			return componentList[i];
	}
	//Simply return a new component of supplied type and throw an error
	cout << "No such component exists on this GameObject. Add one to it first!" << endl;
	return &Component();
}

Component* GameObject::GetComponentAt(int index)
{
	if (index > 0 && index < componentList.size())
		return componentList[index];
	else
	{
		cout << "Invalid index used to getComponentAt()! Are you out of range?" << endl;
		return &Component();
	}
}

void GameObject::AddComponent(Component *source)
{
	componentList.push_back(source);
	source->SetSelfID(this->objectID);
	source->SetMesh(&mesh);
	source->SetTransform(&transform);
	source->SetStandardMaterial(&standardMat);
	source->SetParticleOverlayMaterial(&particleOverlayMat);
	source->Start();
}

void GameObject::RemoveComponents()
{
	for (int i = 0; i < componentList.size(); i++)
	{
		delete componentList[i];
		componentList[i] = nullptr;
	}
	componentList.clear();
}