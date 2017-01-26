#pragma once
#include "CommonIncludes.h"
#include "Transform.h"

//NOTE: Cameras have their own up, right and forward vectors.
//Don't use the ones supplied by Camera.transform.*
//Simply use up right forward within Camera.cpp
class Camera
{
public:
	enum Modes
	{
		MODE_FREE,
		MODE_GAME
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
	mat4 GetViewToProjectionMatrix();

	void SetVerticalFOV(float degrees);
	void SetNearClipPlane(float nearClip);
	void SetFarClipPlane(float farClip);
	
protected:

private:
	float verticalFOV;
	float nearClipPlane;
	float farClipPlane;

	vec3 up;
	vec3 right;
	vec3 forward;
};