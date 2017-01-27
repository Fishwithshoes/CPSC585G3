#pragma once
#include "CommonIncludes.h"
#include "Camera.h"

class Renderer
{
public:
	//TOP LEVEL - GEOMETRY, SHADERS and CAMERA info
	Geometry	geometry;
	Shader		standardShader;
	Shader		particleShader;
	Shader		overlayShader;
	Camera		camera;

	//SHADER UNIFORMS - PRIMARY
	//Standard Shader
	GLint diffuseLevel_uniform;
	GLint diffuseColor_uniform;
	GLint reflectivity_uniform;
	GLint reflectColor_uniform;
	GLint roughness_uniform;
	GLint curveShape_uniform;
	GLint _0degRef_uniform;
	GLint _90degRef_uniform;
	GLint bumpLevel_uniform;
	GLint selfIllumLevel_uniform;
	GLint selfIllumColor_uniform;
	GLint rimLevel_uniform;
	GLint rimColor_uniform;
	GLint rimPower_uniform;
	GLint tileUV_uniform;
	GLint offsetUV_uniform;

	GLint diffuseMap_uniform;
	GLint normalMap_uniform;
	GLint mirrorMap_uniform;
	GLint envMap_uniform;
	GLint roughnessMap_uniform;

	//Particle Shader
	GLint colorParticle_uniform;
	GLint mainTextureParticle_uniform;

	//Overlay Shader
	GLint colorOverlay_uniform;
	GLint mainTextureOverlay_uniform;

	//SHADER UNIFORMS - WORLD INFO
	//Standard Shader
	GLint modelToWorldStandard_uniform;
	GLint worldToViewStandard_uniform;
	GLint viewToProjectionStandard_uniform;
	GLint normalToWorldStandard_uniform;

	GLint cameraPosStandard_uniform;
	GLint lightPosStandard_uniform;
	GLint lightColorStandard_uniform;

	//Particle Shader
	GLint modelToWorldParticle_uniform;
	GLint worldToViewParticle_uniform;
	GLint viewToProjectionParticle_uniform;
	GLint normalToWorldParticle_uniform;

	//Overlay Shader
	GLint modelToWorldOverlay_uniform;
	GLint aspectRatio_uniform;

	//RENDERER PRIMARY OPS
	static void Init(Renderer *renderer);//Setup geometry, shaders and camera(s)
	static void GetShaderUniforms(Renderer *renderer);//For programming shader props
	static void LoadTextures(Renderer *renderer);//Read textures from disk, bind into GPU
	static void RenderScene(Renderer *renderer);//Draw those tris
	static Camera GetCamera(int index);//Grabs the specified camera for editing props

	//RENDERER INPUT LAYOUTS
	static const GLuint POSITION_INDEX = 0;
	static const GLuint COLOR_INDEX = 1;
	static const GLuint NORMAL_INDEX = 2;
	static const GLuint TEXCOORD_INDEX = 3;

protected:

private:
	static vector<Camera> cameraList;
	static vector<string> textureFilePaths;
	static vector<string> cubemapTextureFilePaths;
};