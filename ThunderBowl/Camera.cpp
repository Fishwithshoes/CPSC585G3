#include "Camera.h"
#include "ButtonCode.h"
#include "Input.h"

Camera::Camera(Transform transformIn)
{
	transform = transformIn;
	verticalFOV = Mathf::PI / 180 * 60;
	nearClipPlane = 0.1;
	farClipPlane = 2000.0;
	mode = Modes::MODE_FREE;
	up = Transform::Up();
	right = Transform::Right();
	forward = Transform::Forward();
}

Camera::~Camera()
{
	
}

void Camera::Update()
{
	switch(mode)
	{
	case Modes::MODE_FREE:
		if (Input::GetButton(ButtonCode::A))
		{
			transform.position -= (float)0.001 * right;
		}
		if (Input::GetButton(ButtonCode::D))
		{
			transform.position += (float)0.001 * right;
		}
		if (Input::GetButton(ButtonCode::W))
		{
			transform.position -= (float)0.001 * forward;
		}
		if (Input::GetButton(ButtonCode::S))
		{
			transform.position += (float)0.001 * forward;
		}
		if (Input::GetButton(ButtonCode::E))
		{
			transform.position.y += 0.001;
		}
		if (Input::GetButton(ButtonCode::Q))
		{
			transform.position.y -= 0.001;
		}
		if (Input::GetButton(ButtonCode::RightMouse))
		{
			//Orient camera (pitch and yaw)
			transform.rotation.x -= Input::GetMouseDelta().y * 0.001;
			transform.rotation.y += Input::GetMouseDelta().x * 0.001;

			//Update right and forward vectors
			mat3 pitch(
				1, 0, 0,
				0, cos(-transform.rotation.x), -sin(-transform.rotation.x),
				0, sin(-transform.rotation.x), cos(-transform.rotation.x));

			mat3 yaw(
				cos(-transform.rotation.y), 0, sin(-transform.rotation.y),
				0, 1, 0,
				-sin(-transform.rotation.y), 0, cos(-transform.rotation.y));

			forward = pitch * Transform::Forward();
			right = yaw * Transform::Right();
		}
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
		0, 0, (-nearClipPlane - farClipPlane) / (nearClipPlane - farClipPlane), (2 * farClipPlane*nearClipPlane) / (nearClipPlane - farClipPlane),
		0, 0, 1, 0);
}

void Camera::SetVerticalFOV(float degrees)
{
	verticalFOV = Mathf::PI / 180 * degrees;
}

void Camera::SetNearClipPlane(float near)
{
	nearClipPlane = Mathf::Clamp(near, 0.01, 50);
}

void Camera::SetFarClipPlane(float far)
{
	nearClipPlane = Mathf::Clamp(far, 100, 4000);
}