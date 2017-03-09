#version 450

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec2 VertexTexCoord;

uniform float vertexOffsetScale;
uniform float vertexOffsetPhase;
uniform float vertexRippleScale;
uniform float vertexRipplePhase;
uniform vec4[] vertexRippleOrigins;

out vec3 Position;
out vec3 Color;
out vec3 Normal;
out vec2 TexCoord;
out vec4 ShadowCoord;

uniform mat4 ModelToWorld;
uniform mat4 WorldToView;
uniform mat4 ViewToProjection;
uniform mat4 NormalToWorld;

uniform mat4 WorldToLight;
uniform mat4 LightToProjection;

const float PI = 3.1415926535;

void main()
{
	vec3 newPos = VertexPosition;
	vec3 newNormal = VertexNormal;
	
	//Waves
	newPos.y += sin(VertexPosition.x*0.15*0.5 + VertexPosition.z*0.20*0.5 + vertexOffsetPhase) * vertexOffsetScale * 1.5;
	newPos.y += cos(-VertexPosition.z*0.2*0.5 + VertexPosition.x*0.15*0.5 + vertexOffsetPhase) * vertexOffsetScale * 0.8;
	
	newPos.y += cos(VertexPosition.x*1.5 + vertexOffsetPhase*0.8) * vertexOffsetScale * 0.8;
	newPos.y += sin(-VertexPosition.z*1.2 + vertexOffsetPhase*1.2) * vertexOffsetScale * 1.5;
	
	newNormal.x += sin(VertexPosition.x*0.15*0.5 + VertexPosition.z*0.20*0.5 + vertexOffsetPhase) * vertexOffsetScale * 0.4;
	newNormal.z += cos(-VertexPosition.z*0.2*0.5 + VertexPosition.x*0.15*0.5 + vertexOffsetPhase) * vertexOffsetScale * 0.2;
	
	newNormal.x += cos(VertexPosition.x*1.5 + vertexOffsetPhase*0.8) * vertexOffsetScale * 0.2;
	newNormal.z += sin(-VertexPosition.z*1.2 + vertexOffsetPhase*1.2) * vertexOffsetScale * 0.4;
	
	//Ripple splashes
	float x = VertexPosition.x - vertexRippleOrigins[0].x;
	float z = VertexPosition.z - vertexRippleOrigins[0].z;
	float w = vertexRippleOrigins[0].w;
	float squareRadius = (x * x + z * z) * 0.1;
	float ripple = (sin(15*PI*squareRadius-2*vertexRipplePhase)/(5+100*squareRadius)) * vertexRippleScale * w;
	newPos.y += ripple;	
	newNormal.xz += vec2(ripple * 2.0);
	
	newNormal = normalize(newNormal);

	gl_Position = ViewToProjection * WorldToView * ModelToWorld * vec4(newPos, 1.0);
	
	Position = (ModelToWorld * vec4(newPos, 1.0)).xyz;
    Color = VertexColor;
	Normal = normalize(NormalToWorld * vec4(newNormal, 1.0)).xyz;
	TexCoord = VertexTexCoord;
	
	ShadowCoord = LightToProjection * WorldToLight * ModelToWorld * vec4(VertexPosition, 1.0);
}
