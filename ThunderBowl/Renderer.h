#pragma once
#include "CommonIncludes.h"
#include "Camera.h"

class Renderer
{
public:
	//RENDERER PRIMARY OPS
	//Setup geometry, shaders and camera(s)
	static void Init(Renderer *renderer);
	static void RenderScene(Renderer *renderer);//Draw those tris
	//Grabs the specified camera for editing props
	static Camera* GetCamera(int index);
	//Split screen using count cameras (1-4)
	static void SetCameraCount(int count);
	static int GetCameraCount();
	//Add a point light. Removed every frame.
	static void AddPointLight(PointLight lightDesc);

protected:

private:
	//TOP LEVEL - GEOMETRY, SHADERS and CAMERA info
	Geometry	geometry;
	Shader		standardShader;
	Shader		particleShader;
	Shader		overlayShader;
	Shader		skyboxShader;
	Shader		depthMapShader;
	Shader		fsppShader;
	Camera		camera1;
	Camera		camera2;
	Camera		camera3;
	Camera		camera4;

	//STATIC GEOMETRIES - FOR STATIC OBJECT LIST
	//These are uploaded ONCE during Init(), but can be used by any static world object
	Geometry oceanGeo;
	Geometry oceanGeoDown;
	Geometry puddleGeo;
	Geometry mgbulletGeo;
	Geometry leftWheelGeo;
	Geometry rightWheelGeo;
	Geometry carGeo;
	Geometry carMedGeo;
	Geometry carLowGeo;
	Geometry mapGeo;
	//TODO add any additional high-res items below. Add to the StaticGeos enum too.

	//SHADER UNIFORMS - PRIMARY
	//Standard Shader
	GLint diffuseLevel_uniform;
	GLint diffuseColor_uniform;
	GLint roughness_uniform;
	GLint metalness_uniform;
	GLint isMetallic_uniform;
	GLint transparency_uniform;
	GLint IOR_uniform;
	GLint refractColor_uniform;
	GLint isPhysicalTransparency_uniform;
	GLint bumpLevel_uniform;
	GLint selfIllumLevel_uniform;
	GLint selfIllumColor_uniform;
	GLint rimLevel_uniform;
	GLint rimColor_uniform;
	GLint rimPower_uniform;
	GLint tileUV_uniform;
	GLint offsetUV_uniform;
	GLint fogLevelStandard_uniform;
	GLint vertexOffsetScale_uniform;
	GLint vertexOffsetPhase_uniform;
	GLint vertexRippleScale_uniform;
	GLint vertexRipplePhase_uniform;
	GLint vertexRippleOrigins_uniform;

	GLint diffuseMap_uniform;
	GLint normalMap_uniform;
	GLint mirrorMap_uniform;
	GLint envMap_uniform;
	GLint roughnessMap_uniform;
	GLint metalnessMap_uniform;

	//Particle Shader
	GLint colorParticle_uniform;
	GLint mainTextureParticle_uniform;
	GLint fogLevelParticle_uniform;

	//Overlay Shader
	GLint colorOverlay_uniform;
	GLint mainTextureOverlay_uniform;

	//Skybox Shader
	GLint colorSkybox_uniform;
	GLint envMapSkybox_uniform;

	//SHADER UNIFORMS - WORLD INFO
	//Standard Shader
	GLint modelToWorldStandard_uniform;
	GLint worldToViewStandard_uniform;
	GLint viewToProjectionStandard_uniform;
	GLint normalToWorldStandard_uniform;

	GLint cameraPosStandard_uniform;
	GLint cameraForwardStandard_uniform;
	GLint lightPosStandard_uniform;
	GLint lightColorStandard_uniform;

	//Particle Shader
	GLint modelToWorldParticle_uniform;
	GLint worldToViewParticle_uniform;
	GLint viewToProjectionParticle_uniform;
	GLint normalToWorldParticle_uniform;

	GLint cameraPosParticle_uniform;
	GLint cameraForwardParticle_uniform;

	//Overlay Shader
	GLint modelToWorldOverlay_uniform;
	GLint aspectRatio_uniform;

	//Skybox Shader
	GLint modelToWorldSkybox_uniform;
	GLint worldToViewSkybox_uniform;
	GLint viewToProjectionSkybox_uniform;
	GLint normalToWorldSkybox_uniform;
	GLint cameraPosSkybox_uniform;
	GLint cameraForwardSkybox_uniform;

	//Depth Map Shader
	GLint modelToWorldDepthMap_uniform;
	GLint worldToViewDepthMap_uniform;
	GLint viewToProjectionDepthMap_uniform;

	//Shadow and Deferred buffers
	GLuint shadowBufferID;
	GLuint shadowDepthBufferID;
	static const int shadowSize = 4096;
	GLenum shadowDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLuint framebufferID;
	GLuint depthBufferID;
	GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	//Misc
	static vector<Camera*> cameraList;
	static int cameraCount;
	static vector<string> textureFilePaths;
	static vector<string> cubemapTextureFilePaths;
	static vector<vec4> pointLightColors;
	static vector<vec4> pointLightPositions;

	//RENDERER DELEGATE OPS
	static void LoadTextures(Renderer *renderer);//Read textures from disk, bind into GPU
	static void LoadStaticGeo(Renderer *renderer);//Load high-res geo into GPU
	static void GetShaderUniforms(Renderer *renderer);//For programming shader props
	static void SetupDeferedRendering(Renderer *renderer);//Turns on deffered rendering
	static void SetupShadowMapping(Renderer *renderer);//Turns on shadow mapping
	static void DrawPhysicalObjects(Renderer *renderer, bool programStandardUniforms);
	static void ProgramStandardUniforms(Renderer *renderer, StandardMaterial *stdMat, bool grabPass);
	static void BufferGeoData(Renderer *renderer, Mesh *mesh);//Buffers mesh for dynamic object
	static void BufferStaticGeoData(Geometry *geometry, Mesh *mesh);//Buffers mesh for static object
};