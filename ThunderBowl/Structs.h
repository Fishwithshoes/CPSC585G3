#pragma once
#include "CommonIncludes.h"

using namespace glm;
using namespace std;

//Game Logic
enum GameStates
{
	GS_FRONT_MENU,
	GS_IN_GAME,
	GS_PAUSED,
	GS_GAME_OVER
};

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

enum StaticGeos
{
	SG_OCEAN,
	SG_OCEAN_DOWN,
	SG_PUDDLE,
	SG_MAP
};

enum RenderTypes//Deprecated as of v0.72
{
	RT_EULER,
	RT_QUAT
};

enum Tags
{
	TAGS_DEFAULT,
	//World objects
	TAGS_HUMAN_PLAYER,
	TAGS_AI_PLAYER,
	TAGS_PROJECTILE,
	TAGS_DECORATION,
	TAGS_PARTICLE_SYSTEM,
	//Overlay objects
	TAGS_MENU,
	TAGS_HUD,
	TAGS_PAUSE,
	TAGS_GAME_OVER,
	//Add more as needed...
	max
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

	float roughness;
	float metalness;
	bool isMetallic;//When OFF metalness represents base reflectivity.
	float transparency;
	bool isPhysicalTransparency;//When OFF transparency is simply reverse opacity.

	float bumpLevel;

	float selfIllumLevel;
	vec3 selfIllumColor;

	float rimLevel;
	vec3 rimColor;
	float rimPower;

	vec2 tileUV;
	vec2 offsetUV;

	float fogLevel;

	float vertexOffsetScale;
	float vertexOffsetPhase;

	float vertexRippleScale;
	float vertexRipplePhase;
	vector<vec4> vertexRippleOrigins;

	//Texture handles
	GLuint diffuseMap;
	GLuint normalMap;
	GLuint envMap;
	GLuint mirrorMap;
	GLuint roughnessMap;
	GLuint metalnessMap;
};

//Two separate shaders, but have the same props
struct ParticleOverlayMaterial
{
	vec4 color;
	GLuint mainTexture;

	float fogLevel;
};

//Be sure to bind maps in this order
enum Maps
{
	//Defaults, debugs
	MAP_WHITE,//Default texture color
	MAP_NORMAL,//Default normal color
	MAP_MIRROR,//Testing
	MAP_ENV,//Env cubemap
	MAP_CHECKER,//Testing
	MAP_DEPTH_BUFFER,//Store depth for shadow mapping
	MAP_COLOR_BUFFER,//Store color & alpha for post-process
	MAP_POSITION_BUFFER,//Store normals & distance for post-process
	//Overlay
	MAP_ZERO,
	MAP_ONE,
	MAP_TWO,
	MAP_THREE,
	MAP_FOUR,
	MAP_FIVE,
	MAP_SIX,
	MAP_SEVEN,
	MAP_EIGHT,
	MAP_NINE,
	MAP_SCORE,
	MAP_TIME,
	MAP_JERRY,
	MAP_SPIDER,
	//Particles
	MAP_DEFAULT_PART,
	MAP_BUBBLE_PART,
	MAP_SMOKE_PART,
	//World
	MAP_SUN_DIFFUSE,
	MAP_MOON_DIFFUSE,
	MAP_MOON_NORMAL,
	MAP_WATER_NORMAL,
	MAP_CHASSIS_DIFFUSE,
	MAP_CHASSIS_ROUGHNESS,
	MAP_CHASSIS_NORMAL,
	MAP_WHEEL_DIFFUSE,
	MAP_WHEEL_ROUGHNESS,
	MAP_WHEEL_NORMAL,
	MAP_TRACK_DIFFUSE,
	MAP_TRACK_ROUGHNESS,
	MAP_TRACK_NORMAL
};

//Particles
struct RandRangeFloat
{
	float min;
	float max;
};

struct RandRangeColor
{
	vec4 alpha;
	vec4 bravo;
};

struct Particle
{
	vec3 position;
	vec3 scale;
	vec3 velocity;
	ParticleOverlayMaterial material;
	float startingLife;
	float lifeRemaining;
};

struct ParticleBurst
{
	int count;
	float time;
	bool fired = false;
};

struct ParticleTimeStop
{
	float timePoint;//Relative to particle life (Range = 0-1). Should be unique.
	RandRangeFloat radius;
	RandRangeColor color;
	RandRangeFloat fogLevel;
};

struct ParticleSystemDesc
{
	int							maxParticles;
	float						spawnRate;
	float						coneAngle;
	RandRangeFloat				initialSpeed;
	RandRangeFloat				initialRadius;
	RandRangeColor				initialColor;
	bool						monochromatic;
	GLuint						mainTexture;
	RandRangeFloat				initialFogLevel;
	RandRangeFloat				lifeSpan;
	vec3						spawnPointVariance;
	float						gravityScale;
	float						accelerationScale;
	vector<ParticleBurst>		burstList;
	vector<ParticleTimeStop>	timeStopList;
	bool						useSystemLifespan;
	float						systemLifespan;
	bool						destroySystemWhenEmpty;
};

//Audio WAV info
enum Sounds 
{
	SFX_Hit,
	SFX_Pause
};

enum Musics
{
	MUS_Jerry

};

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