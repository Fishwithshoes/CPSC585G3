#include "Component.h"
#include "Game.h"

void Component::Start()
{
	cout << "I be a Generic Component!" << endl;
}

void Component::Update()
{
	cout << "Tum de dum da doo! You should not be seeing me!!" << endl;
}

void Component::SetMesh(Mesh *mesh)
{
	meshPtr = mesh;
}

void Component::SetTransform(Transform *transform)
{
	transformPtr = transform;
}

void Component::SetStandardMaterial(StandardMaterial *standardMat)
{
	standardMatPtr = standardMat;
}

void Component::SetParticleOverlayMaterial(ParticleOverlayMaterial *particleOverlayMat)
{
	particleOverlayMatPtr = particleOverlayMat;
}

//Call this at start of Start() and Update() to get GameObject info
void Component::Initialize()
{
	mesh = *meshPtr;
	transform = *transformPtr;
	standardMat = *standardMatPtr;
	particleOverlayMat = *particleOverlayMatPtr;
}

//Call this at the end of Start() and Update() to pass changes to GameObject
void Component::Finalize()
{
	*meshPtr = mesh;
	*transformPtr = transform;
	*standardMatPtr = standardMat;
	*transformPtr = transform;
}