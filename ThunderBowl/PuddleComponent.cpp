#include "PuddleComponent.h"

void PuddleComponent::Start()
{
	Initialize();
	cout << "I am a PuddleComponent!" << endl;
	Finalize();
}

void PuddleComponent::Update()
{
	Initialize();
	
	standardMat.vertexRippleScale = rippleHeight;

	currentRippleOffset += rippleSpeed * Time::timeScale * Time::getDeltaTime();
	currentBumpOffset += vec2(-0.0036, 0.003) * rippleSpeed * Time::timeScale * Time::getDeltaTime();

	standardMat.vertexRipplePhase = currentRippleOffset;
	standardMat.offsetUV = currentBumpOffset;

	if (Input::GetButton(ButtonCode::KEY_LEFT))
		standardMat.vertexRippleOrigins[0].x -= Time::getDeltaTime();
	if (Input::GetButton(ButtonCode::KEY_RIGHT))
		standardMat.vertexRippleOrigins[0].x += Time::getDeltaTime();

	if (Input::GetButton(ButtonCode::KEY_UP))
		standardMat.vertexRippleOrigins[0].z += Time::getDeltaTime();
	if (Input::GetButton(ButtonCode::KEY_DOWN))
		standardMat.vertexRippleOrigins[0].z -= Time::getDeltaTime();

	if (Input::GetButton(ButtonCode::O))
		standardMat.vertexRippleOrigins[0].w -= Time::getDeltaTime();
	if (Input::GetButton(ButtonCode::P))
		standardMat.vertexRippleOrigins[0].w += Time::getDeltaTime();

	standardMat.vertexRippleOrigins[0].w = Mathf::Clamp(standardMat.vertexRippleOrigins[0].w, 0, 1);

	

	Finalize();
}