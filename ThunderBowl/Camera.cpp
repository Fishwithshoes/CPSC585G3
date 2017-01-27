#include "Camera.h"
#include "ButtonCode.h"
#include "Input.h"

Camera::Camera()
{
	transform = Transform::identity();
	verticalFOV = Mathf::PI / 180 * 60;
	nearClipPlane = 0.1;
	farClipPlane = 6000.0;
	mode = Modes::MODE_FREE;
	up = Transform::Up();
	right = Transform::Right();
	forward = Transform::Forward();
}
Camera::Camera(Transform transformIn)
{
	transform = transformIn;
	verticalFOV = Mathf::PI / 180 * 60;
	nearClipPlane = 0.1;
	farClipPlane = 6000.0;
	mode = Modes::MODE_FREE;
	up = Transform::Up();
	right = Transform::Right();
	forward = Transform::Forward();
}

Camera::~Camera()
{
	
}

//Use this for initialization
void Camera::Start()
{
	
}

//Use this for behaviour
void Camera::Update()
{
	switch(mode)
	{
	case Modes::MODE_FREE:
		if (Input::GetButton(ButtonCode::A))
		{
			transform.Translate(-(float)0.5 * right);
		}
		if (Input::GetButton(ButtonCode::D))
		{
			transform.Translate((float)0.5 * right);
		}
		if (Input::GetButton(ButtonCode::W))
		{
			transform.Translate(-(float)0.5 * forward);
		}
		if (Input::GetButton(ButtonCode::S))
		{
			transform.Translate((float)0.3 * forward);
		}
		if (Input::GetButton(ButtonCode::E))
		{
			transform.Translate((float)0.5 * up);
		}
		if (Input::GetButton(ButtonCode::Q))
		{
			transform.Translate(-(float)0.5 * up);
		}
		if (Input::GetButton(ButtonCode::RightMouse))
		{
			//Orient camera (pitch and yaw)
			transform.Rotate((float)(Input::GetMouseDelta().y * 0.001) * Transform::Right());
			transform.Rotate((float)(Input::GetMouseDelta().x * 0.001) * Transform::Up());

			//Update right and forward vectors
			mat3 pitch(
				1, 0, 0,
				0, cos(-transform.rotation.x), -sin(-transform.rotation.x),
				0, sin(-transform.rotation.x), cos(-transform.rotation.x));

			mat3 yaw(
				cos(-transform.rotation.y), 0, sin(-transform.rotation.y),
				0, 1, 0,
				-sin(-transform.rotation.y), 0, cos(-transform.rotation.y));

			forward = pitch * yaw * Transform::Forward();
			right = yaw * Transform::Right();
		}
		if (Input::GetButton(ButtonCode::SPACE))
			transform = Transform::Transform(vec3(0, 5, -10), vec3(0), vec3(1));
		break;
	case Modes::MODE_GAME:
		break;
	default:
		break;
	}
}

mat4 Camera::GetWorldToViewMatrix()
{
	mat4 rotationX(
		1, 0, 0, 0,
		0, cos(-transform.rotation.x), -sin(-transform.rotation.x), 0,
		0, sin(-transform.rotation.x), cos(-transform.rotation.x), 0,
		0, 0, 0, 1);

	mat4 rotationY(
		cos(-transform.rotation.y), 0, sin(-transform.rotation.y), 0,
		0,1,0,0,
		-sin(-transform.rotation.y), 0, cos(-transform.rotation.y), 0,
		0, 0, 0, 1);

	mat4 rotationZ(
		cos(-transform.rotation.z), -sin(-transform.rotation.z), 0, 0,
		sin(-transform.rotation.z), cos(-transform.rotation.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	mat4 scale(
		transform.scale.x, 0, 0, 0,
		0, transform.scale.y, 0, 0,
		0, 0, transform.scale.z, 0,
		0, 0, 0, 1);

	mat4 translation(
		1, 0, 0, -transform.position.x,
		0, 1, 0, -transform.position.y,
		0, 0, 1, -transform.position.z,
		0, 0, 0, 1);
	
	return translation * rotationZ * rotationY * rotationX * scale;
}

mat4 Camera::GetViewToProjectionMatrix()
{
	return mat4(
		1/((float)WIDTH/(float)HEIGHT*tan(verticalFOV*0.5)), 0, 0, 0,
		0, 1/tan(verticalFOV*0.5), 0, 0,
		0, 0, (-nearClipPlane - farClipPlane*2) / (nearClipPlane - farClipPlane*2), (4 * farClipPlane*nearClipPlane) / (nearClipPlane - farClipPlane*2),
		0, 0, 1, 0);
}

void Camera::SetVerticalFOV(float degrees)
{
	verticalFOV = Mathf::PI / 180 * degrees;
}

void Camera::SetNearClipPlane(float nearClip)
{
	nearClipPlane = Mathf::Clamp(nearClip, 0.01, 50);
}

void Camera::SetFarClipPlane(float farClip)
{
	nearClipPlane = Mathf::Clamp(farClip, 1000, 12000);
}