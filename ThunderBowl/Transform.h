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
	//These DO NOT update the up, right and forward vectors.
	vec3 position = vec3(0, 0, 0);
	vec3 rotation = vec3(0, 0, 0);
	vec3 scale = vec3(1, 1, 1);

	Transform();
	Transform(vec3 positionIn, vec3 rotationIn, vec3 scaleIn);
	~Transform();

	//Use these when animating object directly.
	//These will update the up, right and forward vectors.
	void Translate(vec3 translation);
	void Rotate(vec3 eulerRotation);
	void Scale(vec3 scalar);
	void LookAt(vec3 point);

	vec3 GetUp();
	vec3 GetRight();
	vec3 GetForward();

protected:
	
private:
	vec3 up;
	vec3 right;
	vec3 forward;
};