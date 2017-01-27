#pragma once
#include "CommonIncludes.h"

using namespace glm;
using namespace std;

//Models
struct Mesh
{
	vector<vec3> positions;
	vector<vec3> colors;
	vector<vec3> normals;
	vector<vec2> texcoords;
	GLsizei elementCount;
	vector<GLuint> indices;
};

struct Geometry
{
	GLuint  positionBuffer;
	GLuint  colorBuffer;
	GLuint  normalBuffer;
	GLuint  texCoordBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;
	GLuint	indexBuffer;
};

//Rendering and Materials
struct Shader
{
	GLuint vertex;
	GLuint fragment;
	GLuint program;
};

struct StandardMaterial
{
	//Props
	float diffuseLevel;
	vec3 diffuseColor;

	float reflectivity;
	vec3 reflectColor;
	float roughness;
	float curveShape;
	float _0degRef;
	float _90degRef;

	float bumpLevel;

	float selfIllumLevel;
	vec3 selfIllumColor;

	float rimLevel;
	vec3 rimColor;
	float rimPower;

	vec2 tileUV;
	vec2 offsetUV;

	//Texture handles
	GLuint diffuseMap;
	GLuint normalMap;
	GLuint envMap;
	GLuint mirrorMap;
	GLuint roughnessMap;
};

//Two separate shaders, but have the same props
struct ParticleOverlayMaterial
{
	vec4 color;
	GLuint mainTexture;
};

//Be sure to bind maps in this order
enum Maps
{
	//Defaults, debugs
	MAP_WHITE,//Default texture color
	MAP_NORMAL,//Default normal color
	MAP_MIRROR,//Default reflection
	MAP_ENV,//Env cubemap
	MAP_CHECKER,//Testing
	//Overlay
	MAP_JERRY,
	//Particles
	//World
	MAP_CHASSIS_DIFFUSE,
	MAP_CHASSIS_ROUGHNESS,
	MAP_CHASSIS_NORMAL,
	MAP_WHEEL_DIFFUSE,
	MAP_WHEEL_ROUGHNESS,
	MAP_WHEEL_NORMAL,
	MAP_TRACK_DIFFUSE,
	MAP_TRACK_ROUGHNESS,
	MAP_TRACK_NORMAL,
};

//Audio WAV info
struct RIFF_Header 
{
	char chunkID[4];
	long chunkSize;
	char format[4];
};

struct WAVE_Format 
{
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

struct WAVE_Data 
{
	char subChunkID[4];
	long subChunk2Size;
};