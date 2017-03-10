#include "Renderer.h"
#include "RendererUtility.h"
#include "Game.h"
#include "GameObject.h"
#include "GeoGenerator.h"
#include "Loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

vector<Camera*> Renderer::cameraList = {};
vector<string> Renderer::textureFilePaths = 
{
	"Textures/white_DIFF.jpg",
	"Textures/normal_NORM.jpg",
	"Textures/reflection_MIRR.jpg",
	"GL_TEXTURE_CUBEMAP",
	"Textures/checker_DIFF.jpg",
	"GL_TEXTURE_DEPTH_BUFFER",
	"GL_TEXTURE_COLOR_BUFFER",
	"GL_TEXTURE_POSITION_BUFFER",
	"GL_TEXTURE_NORMAL_BUFFER",
	"GL_TEXTURE_GRAB_PASS",
	"Textures/zero_TEXT.png",
	"Textures/one_TEXT.png",
	"Textures/two_TEXT.png",
	"Textures/three_TEXT.png",
	"Textures/four_TEXT.png",
	"Textures/five_TEXT.png",
	"Textures/six_TEXT.png",
	"Textures/seven_TEXT.png",
	"Textures/eight_TEXT.png",
	"Textures/nine_TEXT.png",
	"Textures/score_TEXT.png",
	"Textures/time_TEXT.png",
	"Textures/paused_TEXT.png",
	"Textures/gameover_TEXT.png",
	"Textures/start_TEXT.png",
	"Textures/jerry_DIFF.jpg",
	"Textures/SpiderTex.jpg",
	"Textures/default_PART.png",
	"Textures/bubble_PART.png",
	"Textures/smoke_PART.png",
	"Textures/Sun_DIFF.jpg",
	"Textures/Moon_DIFF.jpg",
	"Textures/Moon_NORM.jpg",
	"Textures/water_NORM.jpg",
	"Textures/floor_DIFF.jpg",
	"Textures/floor_DIFF.jpg",
	"Textures/floor_NORM.jpg"
};
vector<string> Renderer::cubemapTextureFilePaths =
{
	"Textures/cube_RIGHT.jpg",
	"Textures/cube_LEFT.jpg",
	"Textures/cube_UP.jpg",
	"Textures/cube_DOWN.jpg",
	"Textures/cube_BACK.jpg",
	"Textures/cube_FRONT.jpg",
};

void Renderer::Init(Renderer *renderer)
{
	LoadTextures(renderer);

	renderer->camera = Camera(Transform(vec3(0, 5, -10), vec4(0, 0, 0, 1), vec3(1)));
	cameraList.push_back(&renderer->camera);

	// call functions to load and compile shader programs
	if (!RendererUtility::InitializeShader(&renderer->standardShader, "Shaders/standardVert.glsl", "Shaders/standardFrag.glsl"))
		cout << "Program could not initialize Standard Shader!" << endl;

	if (!RendererUtility::InitializeShader(&renderer->particleShader, "Shaders/particleVert.glsl", "Shaders/particleFrag.glsl"))
		cout << "Program could not initialize Particle Shader!" << endl;

	if (!RendererUtility::InitializeShader(&renderer->overlayShader, "Shaders/overlayVert.glsl", "Shaders/overlayFrag.glsl"))
		cout << "Program could not initialize Overlay (Front End / HUD) Shader!" << endl;

	if (!RendererUtility::InitializeShader(&renderer->skyboxShader, "Shaders/skyboxVert.glsl", "Shaders/skyboxFrag.glsl"))
		cout << "Program could not initialize Skybox Shader!" << endl;

	if (!RendererUtility::InitializeShader(&renderer->depthMapShader, "Shaders/depthMapVert.glsl", "Shaders/depthMapFrag.glsl"))
		cout << "Program could not initialize Depth Map Shader!" << endl;

	if (!RendererUtility::InitializeShader(&renderer->fsppShader, "Shaders/fsppVert.glsl", "Shaders/fsppFrag.glsl"))
		cout << "Program could not initialize FSPP (Full-Screen Post-Process) Shader!" << endl;

	// call function to create geometry buffers
	if (!RendererUtility::InitializeGeometry(&renderer->geometry))
		cout << "Program failed to intialize geometry!" << endl;

	LoadStaticGeo(renderer);
	GetShaderUniforms(renderer);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glClearColor(0, 0, 0, 0);

	SetupDeferedRendering(renderer);
	SetupShadowMapping(renderer);
}

void Renderer::LoadTextures(Renderer *renderer)
{
	int numComponents;
	unsigned char *data;
	GLuint format;
	GLint loc;
	int width;
	int height;
	GLuint texID;

	for (int i = 0; i < textureFilePaths.size(); i++)
	{
		if (i != MAP_ENV && 
			i != MAP_DEPTH_BUFFER && 
			i != MAP_COLOR_BUFFER && 
			i != MAP_POSITION_BUFFER &&
			i != MAP_NORMAL_BUFFER &&
			i != MAP_GRAB_PASS)//Bind images to GL_TEXTURE_2D
		{
			stbi_set_flip_vertically_on_load(true);
			//Load image with stb and bind into GPU texture table, freeing stb memory afterwards
			data = stbi_load(textureFilePaths[i].c_str(), &width, &height, &numComponents, 0);
			if (data != nullptr)
			{
				format = numComponents == 3 ? GL_RGB : GL_RGBA;
				glGenTextures(1, &texID);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
				cout << "Flumpty failed to load texture at: " + textureFilePaths[i] << endl;
			//stbi_image_free(data);

			cout << "Created texture at: " << texID << endl;
		}
		else if (i == MAP_DEPTH_BUFFER)//This is for shadow mapping
		{
			glGenTextures(1, &texID);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 4096, 4096, 0, GL_RED, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			//glGenerateMipmap(GL_TEXTURE_2D);

			cout << "Created shadow mapping image at: " << texID << "!" << endl;
		}
		else if (i == MAP_COLOR_BUFFER || i == MAP_POSITION_BUFFER || i == MAP_NORMAL_BUFFER)//These are for the Post Process Effects
		{
			glGenTextures(1, &texID);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Camera::WIDTH, Camera::HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glGenerateMipmap(GL_TEXTURE_2D);

			cout << "Created post-process buffer image at: " << texID << "!" << endl;
		}
		else if (i == MAP_GRAB_PASS)//For refraction effects. Filled per object on demand only.
		{
			glGenTextures(1, &texID);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Camera::WIDTH, Camera::HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glGenerateMipmap(GL_TEXTURE_2D);

			cout << "Created grab pass image at: " << texID << "!" << endl;
		}
		else//Bind images to GL_TEXTURE_CUBEMAP_xx for statndard texturing
		{
			stbi_set_flip_vertically_on_load(false);
			glGenTextures(1, &texID);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

			for (int j = 0; j < 6; j++)
			{
				data = stbi_load(cubemapTextureFilePaths[j].c_str(), &width, &height, &numComponents, 0);
				if (data != nullptr)
				{
					format = numComponents == 3 ? GL_RGB : GL_RGBA;
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				}
				else
					cout << "Flumpty failed to load cubemap texture at: " + cubemapTextureFilePaths[j] << endl;
				stbi_image_free(data);
			}

			//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			cout << "Cubemap bound at: " << texID << endl;
		}
	}
}

void Renderer::LoadStaticGeo(Renderer *renderer)
{
	RendererUtility::InitializeGeometry(&renderer->oceanGeo);
	BufferStaticGeoData(&renderer->oceanGeo, &GeoGenerator::MakePlane(2000, 2000, 200, 200, false));

	RendererUtility::InitializeGeometry(&renderer->oceanGeoDown);
	BufferStaticGeoData(&renderer->oceanGeoDown, &GeoGenerator::MakePlane(2000, 2000, 200, 200, true));

	RendererUtility::InitializeGeometry(&renderer->puddleGeo);
	BufferStaticGeoData(&renderer->puddleGeo, &GeoGenerator::MakePlane(10, 10, 100, 100, false));

	Loader loader;
	loader.loadModel("Models/thunderbowl001.obj", vec3(15, 13, 15), true);
	RendererUtility::InitializeGeometry(&renderer->mapGeo);
	BufferStaticGeoData(&renderer->mapGeo, &loader.getMeshes()[0]);
}

void Renderer::GetShaderUniforms(Renderer *renderer)
{
	//PRIMARY SHADER INFO
	//Standard Shader - Primary
	renderer->diffuseLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "diffuseLevel");
	renderer->diffuseColor_uniform = glGetUniformLocation(renderer->standardShader.program, "diffuseColor");
	renderer->roughness_uniform = glGetUniformLocation(renderer->standardShader.program, "roughness");
	renderer->metalness_uniform = glGetUniformLocation(renderer->standardShader.program, "metalness");
	renderer->isMetallic_uniform = glGetUniformLocation(renderer->standardShader.program, "isMetallic");
	renderer->transparency_uniform = glGetUniformLocation(renderer->standardShader.program, "transparency");
	renderer->IOR_uniform = glGetUniformLocation(renderer->standardShader.program, "IOR");
	renderer->refractColor_uniform = glGetUniformLocation(renderer->standardShader.program, "refractColor");
	renderer->isPhysicalTransparency_uniform = glGetUniformLocation(renderer->standardShader.program, "isPhysicalTransparency");
	renderer->bumpLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "bumpLevel");
	renderer->selfIllumLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "selfIllumLevel");
	renderer->selfIllumColor_uniform = glGetUniformLocation(renderer->standardShader.program, "selfIllumColor");
	renderer->rimLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "rimLevel");
	renderer->rimColor_uniform = glGetUniformLocation(renderer->standardShader.program, "rimColor");
	renderer->rimPower_uniform = glGetUniformLocation(renderer->standardShader.program, "rimPower");
	renderer->tileUV_uniform = glGetUniformLocation(renderer->standardShader.program, "tileUV");
	renderer->offsetUV_uniform = glGetUniformLocation(renderer->standardShader.program, "offsetUV");
	renderer->fogLevelStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "fogLevel");
	renderer->vertexOffsetScale_uniform = glGetUniformLocation(renderer->standardShader.program, "vertexOffsetScale");
	renderer->vertexOffsetPhase_uniform = glGetUniformLocation(renderer->standardShader.program, "vertexOffsetPhase");
	renderer->vertexRippleScale_uniform = glGetUniformLocation(renderer->standardShader.program, "vertexRippleScale");
	renderer->vertexRipplePhase_uniform = glGetUniformLocation(renderer->standardShader.program, "vertexRipplePhase");
	renderer->vertexRippleOrigins_uniform = glGetUniformLocation(renderer->standardShader.program, "vertexRippleOrigins");

	renderer->diffuseMap_uniform = glGetUniformLocation(renderer->standardShader.program, "diffuseMap");
	renderer->normalMap_uniform = glGetUniformLocation(renderer->standardShader.program, "normalMap");
	renderer->mirrorMap_uniform = glGetUniformLocation(renderer->standardShader.program, "mirrorMap");
	renderer->envMap_uniform = glGetUniformLocation(renderer->standardShader.program, "envMap");
	renderer->roughnessMap_uniform = glGetUniformLocation(renderer->standardShader.program, "roughnessMap");
	renderer->metalnessMap_uniform = glGetUniformLocation(renderer->standardShader.program, "metalnessMap");
	
	//Particle Shader - Primary
	renderer->colorParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "color");
	renderer->mainTextureParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "mainTexture");
	renderer->fogLevelParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "fogLevel");

	//Overlay Shader - Primary
	renderer->colorOverlay_uniform = glGetUniformLocation(renderer->overlayShader.program, "color");
	renderer->mainTextureOverlay_uniform = glGetUniformLocation(renderer->overlayShader.program, "mainTexture");

	//Skybox Shader - Primary	
	renderer->colorSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "color");
	renderer->envMapSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "envMap");
	
	//WORLD SHADER INFO
	//Standard Shader - World
	renderer->modelToWorldStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "ModelToWorld");
	renderer->worldToViewStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "WorldToView");
	renderer->viewToProjectionStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "ViewToProjection");
	renderer->normalToWorldStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "NormalToWorld");

	renderer->cameraPosStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "cameraPos");
	renderer->cameraForwardStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "cameraForward");
	renderer->lightPosStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "lightPos");
	renderer->lightColorStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "lightColor");

	//Particle Shader - World Info
	renderer->modelToWorldParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "ModelToWorld");
	renderer->worldToViewParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "WorldToView");
	renderer->viewToProjectionParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "ViewToProjection");
	renderer->normalToWorldParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "NormalToWorld");

	renderer->cameraPosParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "cameraPos");
	renderer->cameraForwardParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "cameraForward");

	//Overlay Shader - World Info
	renderer->aspectRatio_uniform = glGetUniformLocation(renderer->overlayShader.program, "AspectRatio");
	renderer->modelToWorldOverlay_uniform = glGetUniformLocation(renderer->overlayShader.program, "ModelToWorld");

	//Skybox Shader - World Info
	renderer->modelToWorldSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "ModelToWorld");
	renderer->worldToViewSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "WorldToView");
	renderer->viewToProjectionSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "ViewToProjection");
	renderer->normalToWorldSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "NormalToWorld");
	renderer->cameraPosSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "cameraPos");
	renderer->cameraForwardSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "cameraForward");

	//Depth (Shadow) Map Shader - WorldInfo
	renderer->modelToWorldDepthMap_uniform = glGetUniformLocation(renderer->depthMapShader.program, "ModelToWorld");
	renderer->worldToViewDepthMap_uniform = glGetUniformLocation(renderer->depthMapShader.program, "WorldToView");
	renderer->viewToProjectionDepthMap_uniform = glGetUniformLocation(renderer->depthMapShader.program, "ViewToProjection");
}

void Renderer::SetupDeferedRendering(Renderer *renderer)
{
	glGenFramebuffers(1, &renderer->framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebufferID);

	//Create and bind a depth buffer
	glGenRenderbuffers(1, &renderer->depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderer->depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Camera::WIDTH, Camera::HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderer->depthBufferID);
	
	//Configure final framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, MAP_COLOR_BUFFER + 1, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, MAP_POSITION_BUFFER + 1, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, MAP_NORMAL_BUFFER + 1, 0);
	glDrawBuffers(3, renderer->drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Flumpty failed to create the post-process framebuffer!" << endl;
}

void Renderer::SetupShadowMapping(Renderer *renderer)
{
	glGenFramebuffers(1, &renderer->shadowBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->shadowBufferID);

	//Create and bind a depth buffer
	glGenRenderbuffers(1, &renderer->shadowDepthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderer->shadowDepthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 4096, 4096);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderer->shadowDepthBufferID);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, MAP_DEPTH_BUFFER + 1, 0);
	glDrawBuffers(1, renderer->shadowDrawBuffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Flumpty failed to create the shadow map framebuffer!" << endl;
}

void Renderer::RenderScene(Renderer *renderer)
{
//**TURN OVER TO THE SHADOW MAP FRAMEBUFFER TO DEPTH MAP PHYSICAL WORLD OBJECTS**********
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->shadowBufferID);
	glViewport(0, 0, 4096, 4096);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderer->depthMapShader.program);

	glCullFace(GL_FRONT);

	//Create WorldToView Matrix for light
	Transform lightTransform = Transform();
	//IMPORTANT: To change light position. Update info in standardFrag and depthMapFrag
	lightTransform.position = vec3(5, 2, 5)*17.0f;
	lightTransform.Rotate(Transform::Up(), 135.0*Mathf::PI/180, false);
	lightTransform.Rotate(lightTransform.GetRight(), -15.0*Mathf::PI/180, false);
	float nearClip = 0.1;
	float farClip = 1000;

	//Calculate an orthographic worldToView and viewToProjection for light's perspective
	mat4 lightWorldToView = glm::lookAt(lightTransform.position, vec3(0), Transform::Up());
	mat4 lightViewToProjection = glm::ortho(-75.0f, 75.0f, -30.0f, 40.0f, nearClip, farClip);
	
	//Program Uniforms for WorldToView and ViewToProjection
	glUniformMatrix4fv(renderer->worldToViewDepthMap_uniform, 1, GL_FALSE, value_ptr(lightWorldToView));
	glUniformMatrix4fv(renderer->viewToProjectionDepthMap_uniform, 1, GL_FALSE, value_ptr(lightViewToProjection));
	
	DrawPhysicalObjects(renderer, false);

	glCullFace(GL_BACK);

//**TURN OVER TO THE DEFERRED FRAMEBUFFER TO DRAW WORLD AND PARTICLES**********
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebufferID);
	glViewport(0, 0, Camera::WIDTH, Camera::HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//**DRAW SKYBOX FIRST AS BACKGROUND**********
	glUseProgram(renderer->skyboxShader.program);
	if(Game::skybox.isVisible)
	{
		//Get skybox
		Mesh mesh = Game::skybox.mesh;
		ParticleOverlayMaterial skyBoxMat = Game::skybox.particleOverlayMat;
		renderer->geometry.elementCount = mesh.indices.size();

		//Program uniforms
		glUniform4fv(renderer->colorSkybox_uniform, 1, value_ptr(skyBoxMat.color));
		glUniform1i(renderer->envMapSkybox_uniform, skyBoxMat.mainTexture);

		glUniformMatrix4fv(renderer->modelToWorldSkybox_uniform, 1, GL_TRUE, value_ptr(Game::skybox.transform.GetModelToWorld()));
		glUniformMatrix4fv(renderer->normalToWorldSkybox_uniform, 1, GL_TRUE, value_ptr(Game::skybox.transform.GetNormalToWorld()));
		//TODO strip out position from camera's WorldToView so the skybox appears at an 'infinite distance'
		glUniformMatrix4fv(renderer->worldToViewSkybox_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetWorldToViewMatrix()));
		glUniformMatrix4fv(renderer->viewToProjectionSkybox_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetViewToProjectionMatrix()));
		glUniform3fv(renderer->cameraPosSkybox_uniform, 1, value_ptr(renderer->camera.transform.position));
		glUniform3fv(renderer->cameraForwardSkybox_uniform, 1, value_ptr(renderer->camera.transform.GetForward()));

		BufferGeoData(renderer, &mesh);
		glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

//**PREPARE TO DRAW ALL WORLD OBJECTS**********
	glUseProgram(renderer->standardShader.program);

	//Program Uniforms for WorldToView and ViewToProjection
	glUniformMatrix4fv(renderer->worldToViewStandard_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetWorldToViewMatrix()));
	glUniformMatrix4fv(renderer->viewToProjectionStandard_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetViewToProjectionMatrix()));

	//Program Other Program Scope Uniforms
	vec3 realCameraPos = renderer->camera.transform.position;
	if (renderer->camera.transform.parent != nullptr)
		realCameraPos = renderer->camera.transform.parent->GetModelToWorld() * vec4(realCameraPos, 1.0);
	glUniform3fv(renderer->cameraPosStandard_uniform, 1, value_ptr(realCameraPos));
	glUniform3fv(renderer->cameraForwardStandard_uniform, 1, value_ptr(renderer->camera.transform.GetForward()));
	glUniform4fv(renderer->lightPosStandard_uniform, 1, value_ptr(vec4(7, 10, -7, 1)));
	glUniform4fv(renderer->lightColorStandard_uniform, 1, value_ptr(vec4(1, 1, 1, 1)));

	//Uniforms for shadow mapping
	GLint shadowMap_uniform = glGetUniformLocation(renderer->standardShader.program, "shadowMap");
	glUniform1i(shadowMap_uniform, MAP_DEPTH_BUFFER);
	GLint worldToLight_uniform = glGetUniformLocation(renderer->standardShader.program, "WorldToLight");
	glUniformMatrix4fv(worldToLight_uniform, 1, GL_FALSE, value_ptr(lightWorldToView));
	GLint lightToProjection_uniform = glGetUniformLocation(renderer->standardShader.program, "LightToProjection");
	glUniformMatrix4fv(lightToProjection_uniform, 1, GL_FALSE, value_ptr(lightViewToProjection));

	DrawPhysicalObjects(renderer, true);

//**RETAIN DEPTH BUFFER AND DRAW PARTICLE SYSTEM OBJECTS**********
	glUseProgram(renderer->particleShader.program);

	//Program Uniforms for WorldToView and ViewToProjection
	glUniformMatrix4fv(renderer->worldToViewParticle_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetWorldToViewMatrix()));
	glUniformMatrix4fv(renderer->viewToProjectionParticle_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetViewToProjectionMatrix()));
	
	//Program Other Program Scope Uniforms
	glUniform3fv(renderer->cameraPosParticle_uniform, 1, value_ptr(realCameraPos));
	glUniform3fv(renderer->cameraForwardParticle_uniform, 1, value_ptr(renderer->camera.transform.GetForward()));

	//Load bilboard mesh into GPU to draw particles with
	{
		Mesh mesh = GeoGenerator::MakeParticle();
		renderer->geometry.elementCount = mesh.indices.size();
		BufferGeoData(renderer, &mesh);
	}
	//For each ParticleSystem GameObject collect the particles into a single vector
	vector<Particle> particleRenderQueue = {};
	for (int i = 0; i < Game::particleObjectList.size(); i++)
	{
		ParticleSystem ps = Game::particleObjectList[i];

		if (ps.isVisible)//Only add to sort queue if the system is visible
		{
			for (int j = 0; j < ps.GetParticleCount(); j++)
			{
				particleRenderQueue.push_back(ps.GetParticles()[j]);
			}
		}
	}
	//Sort particles in queue by depth
	for (int i = 1; i < particleRenderQueue.size(); i++)
	{
		for (int j = 1; j < particleRenderQueue.size(); j++)
		{
			Particle p = particleRenderQueue[j];
			vec3 lengthVec = p.position - renderer->camera.transform.position;
			float dist = dot(lengthVec, lengthVec);

			Particle pp = particleRenderQueue[j - 1];
			lengthVec = pp.position - renderer->camera.transform.position;
			float dist2 = dot(lengthVec, lengthVec);

			if (dist2 < dist)//swap the particles
			{
				particleRenderQueue[j] = pp;
				particleRenderQueue[j - 1] = p;
			}
		}
	}
	//For each particle in the queue, pass uniforms and draw
	for (int i = 0; i < particleRenderQueue.size(); i++)
	{
		Particle p = particleRenderQueue[i];
		ParticleOverlayMaterial particleOverlayMat = p.material;

		//PROGRAM UNIFORMS - PRIMARY
		glUniform4fv(renderer->colorParticle_uniform, 1, value_ptr(particleOverlayMat.color));
		glUniform1i(renderer->mainTextureParticle_uniform, particleOverlayMat.mainTexture);
		glUniform1f(renderer->fogLevelParticle_uniform, particleOverlayMat.fogLevel);

		//PROGRAM UNIFORMS - WORLD INFO
		Transform transform = Transform::identity();
		transform.position = p.position;
		transform.LookAt(renderer->camera.transform.position, false, renderer->camera.transform.GetUp());
		transform.scale = p.scale;
		mat4 modelToWorld = transform.GetModelToWorld();
		mat4 normalToWorld = transform.GetNormalToWorld();
		glUniformMatrix4fv(renderer->modelToWorldParticle_uniform, 1, GL_TRUE, value_ptr(modelToWorld));
		glUniformMatrix4fv(renderer->normalToWorldParticle_uniform, 1, GL_TRUE, value_ptr(normalToWorld));

		glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
		//glDrawElements(GL_LINE_LOOP, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

//**TURN OVER TO THE DEFAULT FRAMEBUFFER TO DRAW POST-PROCESS AND OVERLAY**********
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, Camera::WIDTH, Camera::HEIGHT);

//**CLEAR DEPTH BUFFER AND DRAW POST-PROCESS RECT**********
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderer->fsppShader.program);
	//Program Post-Process Uniforms
	GLint screenDims_uniform = glGetUniformLocation(renderer->fsppShader.program, "screenDims");
	GLint cameraPosPost_uniform = glGetUniformLocation(renderer->fsppShader.program, "cameraPos");
	GLint colorBuffer_uniform = glGetUniformLocation(renderer->fsppShader.program, "colorBufferMap");
	GLint positionBuffer_uniform = glGetUniformLocation(renderer->fsppShader.program, "positionBufferMap");
	GLint normalBuffer_uniform = glGetUniformLocation(renderer->fsppShader.program, "normalBufferMap");
	GLint depthBuffer_uniform = glGetUniformLocation(renderer->fsppShader.program, "depthBufferMap");

	glUniform2f(screenDims_uniform, (float)Camera::WIDTH, (float)Camera::HEIGHT);
	glUniform3fv(cameraPosPost_uniform, 1, value_ptr(realCameraPos));
	glUniform1i(colorBuffer_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBuffer_uniform, MAP_POSITION_BUFFER);
	glUniform1i(normalBuffer_uniform, MAP_NORMAL_BUFFER);
	glUniform1i(depthBuffer_uniform, MAP_DEPTH_BUFFER);
	{
		Mesh mesh = GeoGenerator::MakeRect(2.0, 2.0, GA_CENTER);
		renderer->geometry.elementCount = mesh.indices.size();
		BufferGeoData(renderer, &mesh);
		glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

//**CLEAR DEPTH BUFFER AND DRAW OVERLAY OBJECTS**********
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderer->overlayShader.program);
	glUniform1f(renderer->aspectRatio_uniform, (float)Camera::WIDTH / (float)Camera::HEIGHT);

	//For each Overlay GameObject, Load geometry, bind and draw
	for (int i = 0; i < Game::overlayObjectList.size(); i++)
	{
		//Setup drawing configs
		GameObject gameObject = Game::overlayObjectList[i];
		if (gameObject.isVisible)
		{
			Mesh mesh = gameObject.mesh;
			ParticleOverlayMaterial particleOverlayMat = gameObject.particleOverlayMat;
			renderer->geometry.elementCount = mesh.indices.size();

			//PROGRAM UNIFORMS - PRIMARY
			glUniform4fv(renderer->colorOverlay_uniform, 1, value_ptr(particleOverlayMat.color));
			glUniform1i(renderer->mainTextureOverlay_uniform, particleOverlayMat.mainTexture);

			//PROGRAM UNIFORMS - WORLD INFO
			mat4 modelToWorld = gameObject.GetModelToWorld();
			glUniformMatrix4fv(renderer->modelToWorldOverlay_uniform, 1, GL_TRUE, value_ptr(modelToWorld));

			BufferGeoData(renderer, &mesh);
			glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}

	//Unbind geometry and shader
	glBindVertexArray(0);
	glUseProgram(0);

	// check for and report any OpenGL errors
	//RendererUtility::CheckGLErrors();
}

Camera* Renderer::GetCamera(int index)
{
	return cameraList[index];
}

void Renderer::DrawPhysicalObjects(Renderer *renderer, bool programStandardUniforms)
{
	//DRAW DYNAMIC MESH WORLD OBJECTS
	//For each World GameObject Load geometry into GPU, bind and draw it
	for (int i = 0; i < Game::worldObjectList.size(); i++)
	{
		//Setup drawing configs
		GameObject gameObject = Game::worldObjectList[i];
		Mesh mesh = gameObject.mesh;
		StandardMaterial standardMat = gameObject.standardMat;
		renderer->geometry.elementCount = mesh.indices.size();

		if (programStandardUniforms)
			ProgramStandardUniforms(renderer, &standardMat, gameObject.getGrabPass);

		//Only draw object if doing primary render and is visible OR
		//Doing shadow map pass && casts shadow
		if ((programStandardUniforms && gameObject.isVisible) ||
			(!programStandardUniforms && gameObject.castShadow))
		{
			//PROGRAM UNIFORMS - WORLD INFO
			mat4 modelToWorld = gameObject.GetModelToWorld();
			mat4 normalToWorld = gameObject.GetNormalToWorld();
			if (programStandardUniforms)
			{
				glUniformMatrix4fv(renderer->modelToWorldStandard_uniform, 1, GL_TRUE, value_ptr(modelToWorld));
				glUniformMatrix4fv(renderer->normalToWorldStandard_uniform, 1, GL_TRUE, value_ptr(normalToWorld));
			}
			else
				glUniformMatrix4fv(renderer->modelToWorldDepthMap_uniform, 1, GL_TRUE, value_ptr(modelToWorld));

			BufferGeoData(renderer, &mesh);
			glPointSize(15);
			glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}

	//DRAW STATIC MESH WORLD OBJECTS
	//For each Static World GameObject point to appropriate mesh on GPU and draw
	for (int i = 0; i < Game::staticObjectList.size(); i++)
	{
		GameObject gameObject = Game::staticObjectList[i];
		StandardMaterial standardMat = gameObject.standardMat;

		if (programStandardUniforms)
			ProgramStandardUniforms(renderer, &standardMat, gameObject.getGrabPass);

		//Only draw object if doing primary render and is visible OR
		//Doing shadow map pass && casts shadow
		if ((programStandardUniforms && gameObject.isVisible) ||
			(!programStandardUniforms && gameObject.castShadow))
		{
			//PROGRAM UNIFORMS - WORLD INFO
			mat4 modelToWorld = gameObject.GetModelToWorld();
			mat4 normalToWorld = gameObject.GetNormalToWorld();
			if (programStandardUniforms)
			{
				glUniformMatrix4fv(renderer->modelToWorldStandard_uniform, 1, GL_TRUE, value_ptr(modelToWorld));
				glUniformMatrix4fv(renderer->normalToWorldStandard_uniform, 1, GL_TRUE, value_ptr(normalToWorld));
			}
			else
				glUniformMatrix4fv(renderer->modelToWorldDepthMap_uniform, 1, GL_TRUE, value_ptr(modelToWorld));

			Geometry geoToUse;
			switch (gameObject.staticGeo)
			{
			case SG_OCEAN:
				geoToUse = renderer->oceanGeo;
				break;
			case SG_OCEAN_DOWN:
				geoToUse = renderer->oceanGeoDown;
				break;
			case SG_PUDDLE:
				geoToUse = renderer->puddleGeo;
				break;
			case SG_MAP:
				geoToUse = renderer->mapGeo;
				break;
			default:
				cout << "Invalid RenderMesh specified!" << endl;
				break;
			}

			//Bind and draw with the appropriate arrays
			glBindVertexArray(geoToUse.vertexArray);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geoToUse.indexBuffer);
			glDrawElements(GL_TRIANGLES, geoToUse.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}

void Renderer::ProgramStandardUniforms(Renderer *renderer, StandardMaterial *stdMat, bool grabPass)
{
	//PROGRAM UNIFORMS - PRIMARY
	glUniform1f(renderer->diffuseLevel_uniform, stdMat->diffuseLevel);
	glUniform3fv(renderer->diffuseColor_uniform, 1, value_ptr(stdMat->diffuseColor));
	glUniform1f(renderer->roughness_uniform, stdMat->roughness);
	glUniform1f(renderer->metalness_uniform, stdMat->metalness);
	glUniform1i(renderer->isMetallic_uniform, stdMat->isMetallic ? 1 : 0);
	glUniform1f(renderer->transparency_uniform, stdMat->transparency);
	glUniform1f(renderer->IOR_uniform, stdMat->IOR);
	glUniform3fv(renderer->refractColor_uniform, 1, value_ptr(stdMat->refractColor));
	glUniform1i(renderer->isPhysicalTransparency_uniform, stdMat->isPhysicalTransparency ? 1 : 0);
	glUniform1f(renderer->bumpLevel_uniform, stdMat->bumpLevel);
	glUniform1f(renderer->selfIllumLevel_uniform, stdMat->selfIllumLevel);
	glUniform3fv(renderer->selfIllumColor_uniform, 1, value_ptr(stdMat->selfIllumColor));

	glUniform1f(renderer->rimLevel_uniform, stdMat->rimLevel);
	glUniform3fv(renderer->rimColor_uniform, 1, value_ptr(stdMat->rimColor));
	glUniform1f(renderer->rimPower_uniform, stdMat->rimPower);

	glUniform1i(renderer->diffuseMap_uniform, stdMat->diffuseMap);
	glUniform1i(renderer->normalMap_uniform, stdMat->normalMap);
	glUniform1i(renderer->mirrorMap_uniform, stdMat->mirrorMap);
	glUniform1i(renderer->envMap_uniform, stdMat->envMap);
	glUniform1i(renderer->roughnessMap_uniform, stdMat->roughnessMap);
	glUniform1i(renderer->metalnessMap_uniform, stdMat->metalnessMap);
	if (grabPass)
	{
		glCopyImageSubData(MAP_COLOR_BUFFER+1, GL_TEXTURE_2D, 0, 0, 0, 0, MAP_GRAB_PASS+1, GL_TEXTURE_2D, 0, 0, 0, 0, Camera::WIDTH, Camera::HEIGHT, 1);
	}
	glUniform1i(glGetUniformLocation(renderer->standardShader.program, "grabPassMap"), MAP_GRAB_PASS);
	glUniform2f(glGetUniformLocation(renderer->standardShader.program, "screenDims"), Camera::WIDTH, Camera::HEIGHT);

	glUniform2fv(renderer->tileUV_uniform, 1, value_ptr(stdMat->tileUV));
	glUniform2fv(renderer->offsetUV_uniform, 1, value_ptr(stdMat->offsetUV));
	glUniform1f(renderer->fogLevelStandard_uniform, stdMat->fogLevel);

	glUniform1f(renderer->vertexOffsetScale_uniform, stdMat->vertexOffsetScale);
	glUniform1f(renderer->vertexOffsetPhase_uniform, stdMat->vertexOffsetPhase);
	glUniform1f(renderer->vertexRippleScale_uniform, stdMat->vertexRippleScale);
	glUniform1f(renderer->vertexRipplePhase_uniform, stdMat->vertexRipplePhase);
	glUniform4fv(renderer->vertexRippleOrigins_uniform, stdMat->vertexRippleOrigins.size(), value_ptr(stdMat->vertexRippleOrigins[0]));
}

void Renderer::BufferGeoData(Renderer *renderer, Mesh *mesh)
{
	//BUFFER GEOMETRY AND INDICIES
	glBindVertexArray(renderer->geometry.vertexArray);
	//Buffer Positions
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->positions.size(), mesh->positions.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(RendererUtility::POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::POSITION_INDEX);

	//Buffer Colors
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->colors.size(), mesh->colors.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(RendererUtility::COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::COLOR_INDEX);

	//Buffer Normals
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->normals.size(), mesh->normals.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(RendererUtility::NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::NORMAL_INDEX);

	//Buffer Texcoords
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * mesh->texcoords.size(), mesh->texcoords.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(RendererUtility::TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::TEXCOORD_INDEX);

	//Buffer Indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->geometry.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->indices.size(), mesh->indices.data(), GL_DYNAMIC_DRAW);
}

void Renderer::BufferStaticGeoData(Geometry *geometry, Mesh *mesh)
{
	//BUFFER GEOMETRY AND INDICIES
	glBindVertexArray(geometry->vertexArray);

	geometry->elementCount = mesh->indices.size();
	//Buffer Positions
	glBindBuffer(GL_ARRAY_BUFFER, geometry->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->positions.size(), mesh->positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(RendererUtility::POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::POSITION_INDEX);

	//Buffer Colors
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->colors.size(), mesh->colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(RendererUtility::COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::COLOR_INDEX);

	//Buffer Normals
	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->normals.size(), mesh->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(RendererUtility::NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::NORMAL_INDEX);

	//Buffer Texcoords
	glBindBuffer(GL_ARRAY_BUFFER, geometry->texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * mesh->texcoords.size(), mesh->texcoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(RendererUtility::TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(RendererUtility::TEXCOORD_INDEX);

	//Buffer Indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->indices.size(), mesh->indices.data(), GL_STATIC_DRAW);

	//Unbind buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}