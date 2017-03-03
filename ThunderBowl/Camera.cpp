#include "Camera.h"
#include "ButtonCode.h"
#include "Input.h"

Camera::Camera()
{
	transform = Transform::identity();
	verticalFOV = Mathf::PI / 180 * 60;
	nearClipPlane = 0.1;
	farClipPlane = 10000.0;
	mode = Modes::MODE_FREE;
}
Camera::Camera(Transform transformIn)
{
	transform = transformIn;
	verticalFOV = Mathf::PI / 180 * 60;
	nearClipPlane = 0.1;
	farClipPlane = 10000.0;
	mode = Modes::MODE_FREE;
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
			transform.Translate(-(float)0.5 * transform.GetRight() * panSpeedScale);
		}
		if (Input::GetButton(ButtonCode::D))
		{
			transform.Translate((float)0.5 * transform.GetRight() * panSpeedScale);
		}
		if (Input::GetButton(ButtonCode::W))
		{
			transform.Translate((float)0.5 * transform.GetForward() * panSpeedScale);
		}
		if (Input::GetButton(ButtonCode::S))
		{
			transform.Translate(-(float)0.3 * transform.GetForward() * panSpeedScale);
		}
		if (Input::GetButton(ButtonCode::E))
		{
			transform.Translate((float)0.5 * Transform::Up() * panSpeedScale);
		}
		if (Input::GetButton(ButtonCode::Q))
		{
			transform.Translate(-(float)0.5 * Transform::Up() * panSpeedScale);
		}

		if (Input::GetButton(ButtonCode::A) ||
			Input::GetButton(ButtonCode::D) ||
			Input::GetButton(ButtonCode::W) ||
			Input::GetButton(ButtonCode::S) ||
			Input::GetButton(ButtonCode::E) ||
			Input::GetButton(ButtonCode::Q))
		{
			panSpeedScale += Time::getDeltaTime()*2.0f;
		}
		else
		{
			panSpeedScale = 1.0f;
		}

		if (Input::GetButton(ButtonCode::RIGHT_MOUSE))
		{
			//Orient camera (pitch and yaw)
			transform.Rotate((float)(Input::GetMouseDelta().y * 0.001) * transform.GetRight());
			transform.Rotate((float)(Input::GetMouseDelta().x * 0.001) * Transform::Up());
		}
		if (Input::GetButton(ButtonCode::MIDDLE_MOUSE))
		{
			transform.Rotate((float)(Input::GetMouseDelta().x * 0.001) * transform.GetForward());
		}
		if (Input::GetButton(ButtonCode::SPACE))
		{
			transform = Transform::Transform(vec3(0, 5, -10), vec4(0, 0, 0, 1), vec3(1));

			//transform.position = vec3(5,2,5) * 17.0f;
			//transform.Rotate(Transform::Up(), 135.0*Mathf::PI / 180, false);
			//transform.Rotate(transform.GetRight(), -15.0*Mathf::PI / 180, false);
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
	
	return translation * transform.GetRotationMatrix() * scale;
}

mat4 Camera::GetViewToProjectionMatrix()
{
	return mat4(
		1/((float)Camera::WIDTH/(float)Camera::HEIGHT*tan(verticalFOV*0.5)), 0, 0, 0,
		0, 1/tan(verticalFOV*0.5), 0, 0,
		0, 0, (-nearClipPlane - farClipPlane) / (nearClipPlane - farClipPlane), (2 * farClipPlane*nearClipPlane) / (nearClipPlane - farClipPlane),
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
	farClipPlane = Mathf::Clamp(farClip, 100, 12000);
}