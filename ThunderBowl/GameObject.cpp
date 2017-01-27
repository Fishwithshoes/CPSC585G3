#include "GameObject.h"
#include "CommonIncludes.h"
#include "Game.h"

GameObject::GameObject()
{
	transform = Transform::identity();

	//Initialize Standard Shader Props (Used to draw World Objects)
	standardMat.diffuseLevel = 1.0;
	standardMat.diffuseColor = vec3(1);
	standardMat.reflectivity = 1.0;
	standardMat.reflectColor = vec3(1);
	standardMat.roughness = 0.5;
	standardMat.curveShape = 3.0;
	standardMat._0degRef = 0.2;
	standardMat._90degRef = 1.0;
	standardMat.bumpLevel = 1.0;
	standardMat.selfIllumLevel = 0.0;
	standardMat.selfIllumColor = vec3(1);
	standardMat.rimLevel = 0.0;
	standardMat.rimColor = vec3(1);
	standardMat.rimPower = 2.0;
	standardMat.tileUV = vec2(1, 1);
	standardMat.offsetUV = vec2(0, 0);

	standardMat.diffuseMap = MAP_WHITE;
	standardMat.normalMap = MAP_NORMAL;
	standardMat.mirrorMap = MAP_MIRROR;
	standardMat.envMap = MAP_ENV;
	standardMat.roughnessMap = MAP_WHITE;

	//Initialize ParticleOverlay Shader Props (Used to draw Particles or Overlay)
	particleOverlayMat.color = vec4(1, 1, 1, 1);
	particleOverlayMat.mainTexture = MAP_WHITE;
}

GameObject::~GameObject()
{
	mesh.positions = {};
	mesh.colors = {};
	mesh.normals = {};
	mesh.texcoords = {};
	mesh.elementCount = 0;
	mesh.indices = {};
}

//Use this for initialization
void GameObject::Start()
{
	
}

//Use this for behaviour
void GameObject::Update()
{
	
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
	mat4 start(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

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

	return rotationX * rotationY * rotationZ * start;
}
