#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	scale = vec3(1, 1, 1);
}

Transform::Transform(vec3  positionIn, vec3 rotationIn, vec3 scaleIn)
{
	position = positionIn;
	rotation = rotationIn;
	scale = scaleIn;
}

Transform::~Transform()
{

}

Transform Transform::identity()
{
	return Transform(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1));
}

vec3 Transform::Up()
{
	return vec3(0,1,0);
}

vec3 Transform::Right()
{
	return vec3(1, 0, 0);
}

vec3 Transform::Forward()
{
	return vec3(0, 0, -1);
}

void Transform::LookAt(vec3 point)
{
	
}