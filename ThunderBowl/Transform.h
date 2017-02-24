#pragma once
#include "CommonIncludes.h"

class Transform
{
public:
	static Transform identity();

	static vec3 Up();
	static vec3 Right();
	static vec3 Forward();

	//These public properties are provided for convenience.
	//Use these for updating from PhysX ONLY.
	vec3 position = vec3(0, 0, 0);
	vec4 rotation = vec4(0, 0, 0, 1);
	vec3 scale = vec3(1, 1, 1);

	Transform();
	Transform(vec3 positionIn, vec4 rotationIn, vec3 scaleIn);
	~Transform();

	//Use these when animating object directly.
	void Translate(vec3 translation);
	void Rotate(vec3 eulerAngles);
	void Rotate(vec3 axis, float angle, bool matchTimeScale);
	void Scale(vec3 scalar);
	void LookAt(vec3 point, bool forceUpright, vec3 desiredUp);

	//THESE GET LOCAL ONLY!! Use the ones in GameObject to apply parent transforms too.
	mat4 GetModelToWorld();
	mat4 GetNormalToWorld();

	vec3 GetUp();
	vec3 GetRight();
	vec3 GetForward();
	mat4 GetRotationMatrix();

	vec4 GetInverseRotation();//Returns the inverse of the current rotation

	vec3 up;
	vec3 right;
	vec3 forward;

	Transform *parent = nullptr;

protected:
	
private:
};