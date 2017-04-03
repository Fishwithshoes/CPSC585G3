#include "FloatComponent.h"
#include "Game.h"

//Use this for initialization
void FloatComponent::Start() 
{
	Initialize();
	cout << "I am a FloatComponent!" << endl;
	Finalize();
}

//Use this for behaviour
void FloatComponent::Update()
{
	Initialize();
	
	current += speed * Time::getDeltaTime();
	transform.Translate(vec3(sin(current) * range) * transform.Up(), true);

	//standardMat.roughness = pow(sin(current*0.5),2);

	//for (int i = 0; i < mesh.positions.size(); i++)
	//	mesh.positions[i] += vec3(Random::rangef(-0.001, 0.001),
	//		Random::rangef(-0.001, 0.001), Random::rangef(-0.001, 0.001));

	Finalize();
}