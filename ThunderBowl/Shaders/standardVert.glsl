#version 450

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Color;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelToWorld;
uniform mat4 WorldToView;
uniform mat4 ViewToProjection;
uniform mat4 NormalToWorld;

void main()
{	
	gl_Position = ViewToProjection * WorldToView * ModelToWorld * vec4(VertexPosition, 1.0);
	
	Position = (ModelToWorld * vec4(VertexPosition, 1.0)).xyz;
    Color = VertexColor;
	Normal = normalize(NormalToWorld * vec4(VertexNormal, 1.0)).xyz;
	TexCoord = VertexTexCoord;
}
