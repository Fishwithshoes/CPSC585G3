#version 410

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Color;
out vec3 Normal;
out vec2 TexCoord;

uniform float theta;

uniform mat4 ModelToWorld;
uniform mat4 WorldToView;
uniform mat4 ViewToProjection;
uniform mat4 NormalToWorld;

void main()
{	
	mat3 pitch = mat3(
		1, 0, 0,
		0, cos(theta), -sin(theta),
		0, sin(theta), cos(theta));
	
	mat3 yaw = mat3(
		cos(theta), 0, sin(theta),
		0, 1, 0,
		-sin(theta), 0, cos(theta));
	
	gl_Position = ViewToProjection * WorldToView * ModelToWorld * vec4(yaw * VertexPosition, 1.0);
	
	Position = (ModelToWorld * vec4(yaw * VertexPosition, 1.0)).xyz;
    Color = VertexColor;
	Normal = (NormalToWorld * vec4(yaw * VertexNormal, 1.0)).xyz;
	TexCoord = VertexTexCoord;
}
