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

enum GameWeapons
{
	GW_MACHINE_GUN,
	GW_MISSILE_LAUNCHER,
	GW_FLAMETHROWER
};

enum NodeTypes {
	NT_TRANSIT,
	NT_PLAYER,
	NT_MG_POWERUP,
	NT_FT_POWERUP,
	NT_RK_POWERUP,
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
	//To add a new one:
	//Add it here
	//In Renderer.h (Look for vars named xxGeo)
	//In Renderer::LoadStaticGeo()
	//In Renderer::DrawPhysicalObjects() "Static Mesh" section
	//THE ORDER IS IMPORTANT!!
	SG_OCEAN,
	SG_OCEAN_DOWN,
	SG_PUDDLE,
	SG_MG_BULLET,
	SG_LEFT_WHEEL,
	SG_RIGHT_WHEEL,
	SG_WHEEL,
	SG_CAR,
	SG_CAR_MED,
	SG_CAR_LOW,
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
	//Debug
	TAGS_DEBUG_0,
	TAGS_DEBUG_1,
	TAGS_DEBUG_2,
	TAGS_DEBUG_3,
	TAGS_DEBUG_4,
	//World objects
	TAGS_HUMAN_PLAYER,
	TAGS_AI_PLAYER,
	TAGS_AI_NODE,
	TAGS_PROJECTILE,
	TAGS_MISSILE,
	TAGS_DECORATION,
	TAGS_PARTICLE_SYSTEM,
	TAGS_VEHICLE_WHEEL,
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
	float IOR;//Index of Refraction Default = 1.333 (Water)
	vec3 refractColor;
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

struct PointLight
{
	vec4 Color;//Color and intensity
	vec4 Pos;//Position and radius
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
	MAP_POSITION_BUFFER,//Store position for post-process
	MAP_NORMAL_BUFFER,//Store normal for post-process
	MAP_PREVIOUS_BUFFER,//Store last frame's color buffer for motion blur
	MAP_PARTICLE_FADE,//Store copy of position buffer to depth test particles
	MAP_GRAB_PASS,//Feed Color Buffer into this when an object requests it (For refraction)
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
	MAP_PLAYERS,
	MAP_PAUSED,
	MAP_GAME_OVER,
	MAP_START,
	MAP_ARROW_ICON,
	MAP_HEALTH_ICON,
	MAP_MACHINE_GUN_ICON,
	MAP_MISSILE_LAUNCHER_ICON,
	MAP_FLAMETHROWER_ICON,
	MAP_RING01_ICON,
	MAP_RING02_ICON,
	MAP_RING03_ICON,
	MAP_RING04_ICON,
	MAP_DEFAULT_GRAD,
	MAP_JHCBLUE_GRAD,
	MAP_JHCGRAY_GRAD,
	MAP_JERRY,
	MAP_SPIDER,
	//Particles
	MAP_DEFAULT_PART,
	MAP_BUBBLE_PART,
	MAP_SMOKE_PART,
	MAP_FLAME01_PART,
	MAP_FLAME02_PART,
	MAP_FLAME03_PART,
	MAP_FLAME04_PART,
	MAP_FLAME05_PART,
	MAP_FLAME06_PART,
	MAP_FLAME07_PART,
	MAP_FLAME08_PART,
	MAP_FLAME09_PART,
	MAP_FLAME10_PART,
	MAP_FLAME11_PART,
	MAP_FLAME12_PART,
	MAP_FLAME13_PART,
	MAP_FLAME14_PART,
	MAP_FLAME15_PART,
	MAP_FLAME16_PART,
	MAP_SPARK01_PART,
	MAP_SPARK02_PART,
	MAP_SPARK03_PART,
	MAP_SPARK04_PART,
	MAP_SPARK05_PART,
	MAP_SPARK06_PART,
	MAP_SPARK07_PART,
	MAP_SPARK08_PART,
	MAP_FLASH_PART,
	//World
	MAP_SUN_DIFFUSE,
	MAP_MOON_DIFFUSE,
	MAP_MOON_NORMAL,
	MAP_WATER_NORMAL,
	MAP_TRACK_DIFFUSE,
	MAP_TRACK_ROUGHNESS,
	MAP_TRACK_NORMAL,
	MAP_CHASSIS_DIFFUSE,
	MAP_CHASSIS_ROUGHNESS,
	MAP_CHASSIS_METALNESS,
	MAP_CHASSIS_NORMAL,
	MAP_WHEEL_DIFFUSE,
	MAP_WHEEL_ROUGHNESS,
	MAP_WHEEL_METALNESS,
	MAP_WHEEL_NORMAL,
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
	float						coneAngle;//In degrees
	RandRangeFloat				initialSpeed;
	RandRangeFloat				initialRadius;
	RandRangeColor				initialColor;
	bool						monochromatic;
	GLuint						mainTexture;//Used if length of textures is 0
	vector<GLuint>				textures;//Used for randomly assigned birth textures
	RandRangeFloat				initialFogLevel;
	RandRangeFloat				lifeSpan;
	vec3						spawnPointVariance;
	float						gravityScale;
	float						accelerationScale;
	float						scaleScale;
	bool						useParticleLights;
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
	SFX_Dent,
	SFX_Pause,
	SFX_Select,
	SFX_SwitchWeapon,
	SFX_MG,
	SFX_Powerup,
	SFX_FireMissile,
	SFX_ExplodeMissile,
	SFX_ExplodeCar,
	SFX_Flamethrower,
	SFX_Splish,
	SFX_Splash,
	SFX_Engine,
	SFX_Engine1,
	SFX_Engine2,
	SFX_Engine3,
	SFX_Engine4,
	SFX_Engine5,
	SFX_Engine6,
	SFX_Engine7,
	SFX_Engine8,
	SFX_Engine9,
	SFX_Horn

};

enum Musics
{
	MUS_Automation,
	MUS_Conta,
	MUS_Menu,
	MUS_Battle,
	MUS_BloodMoon
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