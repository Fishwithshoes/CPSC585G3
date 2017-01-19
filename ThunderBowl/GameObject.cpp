#include "GameObject.h"
#include "CommonIncludes.h"

GameObject::GameObject()
{
	mesh.elementCount = 0;
	mesh.indicies = {};
}

GameObject::~GameObject()
{
	mesh.positions = {};
	mesh.colors = {};
	mesh.normals = {};
	mesh.texcoords = {};
	mesh.elementCount = 0;
	mesh.indicies = {};
}

mat4 GameObject::GetModelToWorld()
{
	mat4 rotationX(
		1, 0, 0, 0,
		0, cos(transform.rotation.x), -sin(transform.rotation.x), 0,
		0, sin(transform.rotation.x), cos(transform.rotation.x), 0,
		0, 0, 0, 1);

	mat4 rotationY(
		cos(transform.rotation.y), 0, sin(transform.rotation.y), 0,
		0, 1, 0, 0,
		-sin(transform.rotation.y), 0, cos(transform.rotation.y), 0,
		0, 0, 0, 1);

	mat4 rotationZ(
		cos(transform.rotation.z), -sin(transform.rotation.z), 0, 0,
		sin(transform.rotation.z), cos(transform.rotation.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	mat4 scale(
		transform.scale.x, 0, 0, 0,
		0, transform.scale.y, 0, 0,
		0, 0, transform.scale.z, 0,
		0, 0, 0, 1);

	mat4 translation(
		1, 0, 0, transform.position.x,
		0, 1, 0, transform.position.y,
		0, 0, 1, transform.position.z,
		0, 0, 0, 1);

	return translation * rotationX * rotationY * rotationZ * scale;
}

mat4 GameObject::GetNormalToWorld()
{
	mat4 rotationX(
		1, 0, 0, 0,
		0, cos(transform.rotation.x), -sin(transform.rotation.x), 0,
		0, sin(transform.rotation.x), cos(transform.rotation.x), 0,
		0, 0, 0, 1);

	mat4 rotationY(
		cos(transform.rotation.y), 0, sin(transform.rotation.y), 0,
		0, 1, 0, 0,
		-sin(transform.rotation.y), 0, cos(transform.rotation.y), 0,
		0, 0, 0, 1);

	mat4 rotationZ(
		cos(transform.rotation.z), -sin(transform.rotation.z), 0, 0,
		sin(transform.rotation.z), cos(transform.rotation.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return rotationX * rotationY * rotationZ;
}
