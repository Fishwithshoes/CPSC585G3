#version 450

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec2 VertexTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform float AspectRatio;
uniform mat4 ModelToWorld;

void main()
{
	gl_Position = ModelToWorld * vec4(VertexPosition.x/AspectRatio, VertexPosition.y, VertexPosition.z, 1.0);
	
	Color = VertexColor;
	TexCoord = VertexTexCoord;
}