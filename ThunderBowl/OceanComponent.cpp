#include "OceanComponent.h"

void OceanComponent::Start()
{
	Initialize();
	cout << "I am an OceanComponent!" << endl;
	Finalize();
}

void OceanComponent::Update()
{
	Initialize();

	standardMat.vertexOffsetScale = waveHeight;

	currentVertOffset += currentSpeed * Time::timeScale * Time::getDeltaTime();
	currentBumpOffset += vec2(-0.018,  0.015) * currentSpeed * Time::timeScale * Time::getDeltaTime();

	standardMat.vertexOffsetPhase = currentVertOffset;
	standardMat.offsetUV = currentBumpOffset;

	Finalize();
}