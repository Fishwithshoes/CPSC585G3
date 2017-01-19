#pragma once
#include "CommonIncludes.h"
#include "Transform.h"

class Camera
{
public:
	enum Modes
	{
		MODE_FREE,
		MODE_GAME
	};

	//Properties for screen (big camera) NOT individual cameras
	static const int WIDTH = 960;
	static const int HEIGHT = 540;

	//Properties for individual cameras
	Transform transform;
	Modes mode;

	Camera(Transform transformIn);
	~Camera();

	void Update();

	mat4 GetWorldToViewMatrix();
	mat4 GetViewToProjectionMatrix();

	void SetVerticalFOV(float degrees);
	void SetNearClipPlane(float near);
	void SetFarClipPlane(float far);
	
protected:

private:
	float verticalFOV;
	float nearClipPlane;
	float farClipPlane;

	vec3 up;
	vec3 right;
	vec3 forward;
};