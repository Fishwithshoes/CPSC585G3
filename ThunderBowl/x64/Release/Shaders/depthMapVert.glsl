#version 450

layout(location = 0) in vec3 VertexPosition;

uniform mat4 ModelToWorld;
uniform mat4 WorldToView;
uniform mat4 ViewToProjection;

out vec3 Position;

void main(void)
{
	gl_Position = ViewToProjection * WorldToView * ModelToWorld * vec4(VertexPosition, 1.0);
	
	Position = (ModelToWorld * vec4(VertexPosition, 1.0)).xyz;
}