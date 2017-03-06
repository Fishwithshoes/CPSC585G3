#include "Transform.h"
#include "Renderer.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec4(0, 0, 0, 1);
	scale = vec3(1, 1, 1);
}

Transform::Transform(vec3  positionIn, vec4 rotationIn, vec3 scaleIn)
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

void Transform::Translate(vec3 translation, bool matchTimeScale)
{
	if (matchTimeScale)
		translation = translation * (float)Time::timeScale;
	position += translation;
}

void Transform::Rotate(vec3 eulerAngles, bool matchTimeScale)
{
	//Match with timeScale
	if(matchTimeScale)
		eulerAngles = eulerAngles * Time::timeScale;

	//Convert input rotation into quaternion
	vec4 newRot;
	float c1 = cos(eulerAngles.y*0.5);
	float c2 = cos(eulerAngles.x*0.5);
	float c3 = cos(eulerAngles.z*0.5);
	float s1 = sin(eulerAngles.y*0.5);
	float s2 = sin(eulerAngles.x*0.5);
	float s3 = sin(eulerAngles.z*0.5);
	
	newRot.x = c1*s2*c3 - s1*c2*s3;
	newRot.y = s1*c2*c3 + c1*s2*s3;
	newRot.z = s1*s2*c3 + c1*c2*s3;
	newRot.w = c1*c2*c3 - s1*s2*s3;

	//Multiply the two quaternions
	vec4 q = rotation;
	vec4 r = newRot;

	rotation.x = (r.w*q.x + r.x*q.w - r.y*q.z + r.z*q.y);
	rotation.y = (r.w*q.y + r.x*q.z + r.y*q.w - r.z*q.x);
	rotation.z = (r.w*q.z - r.x*q.y + r.y*q.x + r.z*q.w);
	rotation.w = (r.w*q.w - r.x*q.x - r.y*q.y - r.z*q.z);
}

void Transform::Rotate(vec3 axis, float angle, bool matchTimeScale)
{
	//Match with timeScale
	if(matchTimeScale)
		angle = angle * Time::timeScale;

	//Construct quaternion from angle and vector of rotation
	vec4 newRot;
	newRot.x = axis.x * sin(angle * 0.5);
	newRot.y = axis.y * sin(angle * 0.5);
	newRot.z = axis.z * sin(angle * 0.5);
	newRot.w = cos(angle * 0.5);

	//Multiply current by the new for a new orientation
	vec4 q = rotation;
	vec4 r = newRot;

	rotation.x = (r.w*q.x + r.x*q.w - r.y*q.z + r.z*q.y);
	rotation.y = (r.w*q.y + r.x*q.z + r.y*q.w - r.z*q.x);
	rotation.z = (r.w*q.z - r.x*q.y + r.y*q.x + r.z*q.w);
	rotation.w = (r.w*q.w - r.x*q.x - r.y*q.y - r.z*q.z);
}

void Transform::Scale(vec3 scalar, bool matchTimeScale)
{
	if (matchTimeScale)
		scalar = scalar * (float)Time::timeScale;
	scale += scalar;
}

mat4 Transform::GetModelToWorld()
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

	return scale * GetRotationMatrix() * translation;
}

mat4 Transform::GetNormalToWorld()
{
	return GetRotationMatrix();
}

void Transform::LookAt(vec3 point, bool forceUpright, vec3 desiredUp)
{
	//Get angle between vectors
	vec3 a = GetForward();
	vec3 b = normalize(point - position);
	float dotProduct = dot(a, b);

	//Check for vector alignment. Assign final angle and vector of rotation
	float rotAngle = acos(dotProduct);
	vec3 rotAxis;

	if (dotProduct > 0.999 && dotProduct < 1.001)
	{
		//Already looking at it
		rotAxis = Up();
	}
	else if (dotProduct < -0.999 && dotProduct > -1.001)
	{
		//Looking directly away
		rotAxis = Up();
	}
	else
	{
		rotAxis = normalize(cross(a, b));
	}

	Rotate(rotAxis, rotAngle, false);

	if (forceUpright)
	{
		a = GetUp();
		b = normalize(desiredUp);

		dotProduct = dot(a, b);

		rotAngle = acos(dotProduct);
		rotAxis = -GetForward();

		Rotate(rotAxis, rotAngle, false);
	}
}

vec3 Transform::GetUp()
{
	return GetRotationMatrix() * vec4(Up(), 1.0);
}

vec3 Transform::GetRight()
{
	return GetRotationMatrix() * vec4(Right(), 1.0);
}

vec3 Transform::GetForward()
{
	return GetRotationMatrix() * vec4(Forward(), 1.0);
}

mat4 Transform::GetRotationMatrix()
{
	vec4 q = rotation;
	mat4 rotation(
		1 - 2 * q.y*q.y - 2 * q.z*q.z, 2 * q.x*q.y - 2 * q.z*q.w, 2 * q.x*q.z + 2 * q.y*q.w, 0,
		2 * q.x*q.y + 2 * q.z*q.w, 1 - 2 * q.x*q.x - 2 * q.z*q.z, 2 * q.y*q.z - 2 * q.x*q.w, 0,
		2 * q.x*q.z - 2 * q.y*q.w, 2 * q.y*q.z + 2 * q.x*q.w, 1 - 2 * q.x*q.x - 2 * q.y*q.y, 0,
		0, 0, 0, 1);

	return rotation;
}

vec4 Transform::GetInverseRotation()
{
	vec4 result;

	float s = rotation.x*rotation.x+rotation.y*rotation.y+rotation.z*rotation.z+rotation.w*rotation.w;
	result.x = -rotation.x / s;
	result.y = -rotation.y / s;
	result.z = -rotation.z / s;
	result.w = rotation.w / s;

	return result;
}