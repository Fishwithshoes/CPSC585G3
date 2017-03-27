#pragma once
#include "CommonIncludes.h"
#include "Transform.h"

class Camera
{
public:
	enum Modes
	{
		MODE_FREE,
		MODE_GAME,
		MODE_PRESENTATION
	};

	//Properties for screen (big camera) NOT individual cameras
	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;

	//Properties for individual cameras
	Transform transform;
	Modes mode;

	Camera();
	Camera(Transform transformIn);
	~Camera();

	void Start();
	void Update();

	mat4 GetWorldToViewMatrix();
	mat4 GetViewToProjectionMatrix(vec2 aspect);
	RenderTypes rendertype = RenderTypes::RT_EULER;

	void SetVerticalFOV(float degrees);
	void SetNearClipPlane(float nearClip);
	void SetFarClipPlane(float farClip);
	
protected:

private:
	float verticalFOV;
	float nearClipPlane;
	float farClipPlane;

	float panSpeedScale = 1.0;

	Transform orbitTransform;
};