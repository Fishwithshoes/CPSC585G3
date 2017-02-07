#pragma once
#include "Structs.h"
#include "Transform.h"
#include "Component.h"
#include "FloatComponent.h"
#include "RotateComponent.h"
#include "VehicleComponent.h"

class GameObject
{
public:
	Mesh mesh;
	Transform transform;
	StandardMaterial standardMat;
	ParticleOverlayMaterial particleOverlayMat;
	string name;//Should be unique
	Tags tag;//Intended for group classification

	GameObject();
	GameObject(Mesh meshIn, string nameIn, Tags tagIn);
	~GameObject();

	void Start();
	void Update();

	mat4 GetModelToWorld();
	mat4 GetNormalToWorld();

	Component* GetComponent(Component* type);//Returns first compoent of type
	Component* GetComponentAt(int index);//Returns component at index
	void AddComponent(Component *source);//Adds the supplied component to GameObject
	void RemoveComponents();//Removes all components from the GameObject

	//DO NOT modify, unless Creating or Destroying objects.
	//This Should ONLY be done in Game.CreateXXX() and Game.DestroyXXX()
	int objectID = -1;

protected:

private:
	vector<Component*> componentList;
};
