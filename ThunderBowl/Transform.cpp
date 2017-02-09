#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	scale = vec3(1, 1, 1);

	up = Up();
	right = Right();
	forward = Forward();
}

Transform::Transform(vec3  positionIn, vec3 rotationIn, vec3 scaleIn)
{
	Translate(positionIn);
	Rotate(rotationIn);
	Scale(scaleIn);

	up = Up();
	right = Right();
	forward = Forward();
}

Transform::~Transform()
{

}

Transform Transform::identity()
{
	return Transform();
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
	return vec3(0, 0, 1);
}

void Transform::Translate(vec3 translation)
{
	position += translation * (float)Time::timeScale;
}

void Transform::Rotate(vec3 eulerRotation)
{
	rotation += eulerRotation * (float)Time::timeScale;

	//Update local up, right and forward vectors
	mat3 pitch(
		1, 0, 0,
		0, cos(-rotation.x), -sin(-rotation.x),
		0, sin(-rotation.x), cos(-rotation.x));

	mat3 yaw(
		cos(-rotation.y), 0, sin(-rotation.y),
		0, 1, 0,
		-sin(-rotation.y), 0, cos(-rotation.y));

	mat3 roll(
		cos(-rotation.x), -sin(-rotation.x), 0,
		sin(-rotation.x), cos(-rotation.x), 0,
		0, 0, 1);

	up = roll * pitch * Up();
	right = roll * yaw * Right();
	forward = yaw * pitch * Forward();
}

void Transform::Scale(vec3 scalar)
{
	scale += scalar * (float)Time::timeScale;
}

mat4 Transform::GetModelToWorld()
{
	mat4 rotationX(
		1, 0, 0, 0,
		0, cos(rotation.x), -sin(rotation.x), 0,
		0, sin(rotation.x), cos(rotation.x), 0,
		0, 0, 0, 1);

	mat4 rotationY(
		cos(rotation.y), 0, sin(rotation.y), 0,
		0, 1, 0, 0,
		-sin(rotation.y), 0, cos(rotation.y), 0,
		0, 0, 0, 1);

	mat4 rotationZ(
		cos(rotation.z), -sin(rotation.z), 0, 0,
		sin(rotation.z), cos(rotation.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	mat4 scale(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);

	mat4 translation(
		1, 0, 0, position.x,
		0, 1, 0, position.y,
		0, 0, 1, position.z,
		0, 0, 0, 1);

	return scale * rotationX * rotationY * rotationZ * translation;
}

mat4 Transform::GetNormalToWorld()
{
	mat4 start(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	mat4 rotationX(
		1, 0, 0, 0,
		0, cos(rotation.x), -sin(rotation.x), 0,
		0, sin(rotation.x), cos(rotation.x), 0,
		0, 0, 0, 1);

	mat4 rotationY(
		cos(rotation.y), 0, sin(rotation.y), 0,
		0, 1, 0, 0,
		-sin(rotation.y), 0, cos(rotation.y), 0,
		0, 0, 0, 1);

	mat4 rotationZ(
		cos(rotation.z), -sin(rotation.z), 0, 0,
		sin(rotation.z), cos(rotation.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return rotationX * rotationY * rotationZ * start;
}

mat4 Transform::GetQuatModelToWorld()
{

	mat4 scale(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);

	mat4 translation(
		1, 0, 0, position.x,
		0, 1, 0, position.y,
		0, 0, 1, position.z,
		0, 0, 0, 1);

	return scale * rotationMatrix * translation;
}

mat4 Transform::GetQuatNormalToWorld()
{
	return rotationMatrix;
}

void Transform::LookAt(vec3 point)
{

}

vec3 Transform::GetUp()
{
	return up;
}

vec3 Transform::GetRight()
{
	return right;
}

vec3 Transform::GetForward()
{
	return forward;
}