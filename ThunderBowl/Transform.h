#pragma once
#include "CommonIncludes.h"

class Transform
{
public:
	static Transform identity();

	vec3 position = vec3(0, 0, 0);
	vec3 rotation = vec3(0, 0, 0);
	vec3 scale = vec3(1, 1, 1);

	Transform();
	Transform(vec3 positionIn, vec3 rotationIn, vec3 scaleIn);
	~Transform();

	static vec3 Up();
	static vec3 Right();
	static vec3 Forward();

	void LookAt(vec3 point);

protected:
	
private:

};