#include "GameObject.h"
#include "CommonIncludes.h"
#include "Game.h"

//Only use this Constructor for objects you don't really care about...
GameObject::GameObject()
{
	transform = Transform::identity();
	SetupDefaultMaterials();
	name = "Default";
	tag = Tags::TAGS_DEFAULT;
}

//Use this Constructor for more "Important" objects that you may want access to later (most)
GameObject::GameObject(string nameIn, Tags tagIn)
{
	transform = Transform::identity();
	SetupDefaultMaterials();
	name = nameIn;
	tag = tagIn;
}

void GameObject::SetupDefaultMaterials()
{
	isVisible = true;
	castShadow = true;
	getGrabPass = false;

	//Initialize Standard Shader Props (Used to draw World Objects)
	standardMat.diffuseLevel = 1.0;
	standardMat.diffuseColor = vec3(1);
	standardMat.roughness = 0.5;
	standardMat.metalness = 0.04;
	standardMat.isMetallic = false;
	standardMat.transparency = 0.0;
	standardMat.IOR = 1.333;
	standardMat.refractColor = vec3(1);
	standardMat.isPhysicalTransparency = true;
	standardMat.bumpLevel = 1.0;
	standardMat.selfIllumLevel = 0.0;
	standardMat.selfIllumColor = vec3(1);
	standardMat.rimLevel = 0.0;
	standardMat.rimColor = vec3(1);
	standardMat.rimPower = 2.0;
	standardMat.tileUV = vec2(1, 1);
	standardMat.offsetUV = vec2(0, 0);
	standardMat.fogLevel = 1.0;
	standardMat.vertexOffsetScale = 0.0;
	standardMat.vertexOffsetPhase = 0.0;
	standardMat.vertexRippleScale = 0.0;
	standardMat.vertexRipplePhase = 0.0;
	standardMat.vertexRippleOrigins = {};
	for (int i = 0; i < 10; i++)
		standardMat.vertexRippleOrigins.push_back(vec4());

	standardMat.diffuseMap = MAP_WHITE;
	standardMat.normalMap = MAP_NORMAL;
	standardMat.envMap = MAP_ENV;
	standardMat.roughnessMap = MAP_WHITE;
	standardMat.metalnessMap = MAP_WHITE;

	//Initialize ParticleOverlay Shader Props (Used to draw Particles, Overlay and skybox)
	particleOverlayMat.color = vec4(1, 1, 1, 1);
	particleOverlayMat.mainTexture = MAP_WHITE;
	particleOverlayMat.fogLevel = 1.0;
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

	//if (name == "Billet" && GetComponent(&FloatComponent()))
	//	transform.Translate(vec3(0, 1, 0) * Time::getDeltaTime(), true);
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
	if (transform.parent == nullptr)
	{
		return transform.GetNormalToWorld();
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
	//cout << "No such component exists on this GameObject. Add one to it first!" << endl;
	//cout << name << endl;
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
	source->SetSelfID(&objectID);
	source->SetSelfName(&name);
	source->SetMesh(&mesh);
	source->SetTransform(&transform);
	source->SetStandardMaterial(&standardMat);
	source->SetParticleOverlayMaterial(&particleOverlayMat);
	source->Start();
	componentList.push_back(source);
}

void GameObject::RemoveComponent(Component* type)
{
	//Search for component of supplied type and remove the first instance
	for (int i = 0; i < componentList.size(); i++)
	{
		if (typeid(*componentList[i]) == typeid(*type))
		{
			delete componentList[i];
			componentList[i] = nullptr;
			componentList.erase(componentList.begin() + i);
			return;
		}
	}
	cout << "ERROR. Unable to remove component from: "<< name <<"! No such type found" << endl;
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