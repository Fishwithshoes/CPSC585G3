#pragma once
#include "CommonIncludes.h"
#include "Transform.h"
#include "Input.h"
#include "ButtonCode.h"

class Component
{
public:
	virtual void Start();
	virtual void Update();

	void SetSelfID(int *selfID);
	void SetSelfName(string *selfName);
	void SetMesh(Mesh *mesh);
	void SetTransform(Transform *transform);
	void SetStandardMaterial(StandardMaterial *standardMat);
	void SetParticleOverlayMaterial(ParticleOverlayMaterial *particleOverlayMat);

	void Initialize();//Call this at start of Start() and Update() to get GameObject info
	void Finalize();//Call this at the end of Start() and Update() to pass changes to GameObject

protected:
	int selfID;
	string selfName;
	Mesh mesh;
	Transform transform;
	StandardMaterial standardMat;
	ParticleOverlayMaterial particleOverlayMat;

private:
	int *selfIDPtr;
	string *selfNamePtr;
	Mesh *meshPtr;
	Transform *transformPtr;
	StandardMaterial *standardMatPtr;
	ParticleOverlayMaterial *particleOverlayMatPtr;
};

