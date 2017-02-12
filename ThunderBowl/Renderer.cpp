#include "Renderer.h"
#include "RendererUtility.h"
#include "Game.h"
#include "GameObject.h"
#include "GeoGenerator.h"
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
	"GL_TEXTURE_COLOR_BUFFER",
	"GL_TEXTURE_POSITION_BUFFER",
	"Textures/jerry_DIFF.jpg",
	"Textures/SpiderTex.jpg",
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
	renderer->camera = Camera(Transform(vec3(0, 5, -10), vec3(0), vec3(1)));
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

	if (!RendererUtility::InitializeShader(&renderer->fsppShader, "Shaders/fsppVert.glsl", "Shaders/fsppFrag.glsl"))
		cout << "Program could not initialize FSPP (Full-Screen Post-Process) Shader!" << endl;

	// call function to create geometry buffers
	if (!RendererUtility::InitializeGeometry(&renderer->geometry))
		cout << "Program failed to intialize geometry!" << endl;
}

void Renderer::GetShaderUniforms(Renderer *renderer)
{
	//PRIMARY SHADER INFO
	//Standard Shader - Primary
	renderer->diffuseLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "diffuseLevel");
	renderer->diffuseColor_uniform = glGetUniformLocation(renderer->standardShader.program, "diffuseColor");
	renderer->reflectivity_uniform = glGetUniformLocation(renderer->standardShader.program, "reflectivity");
	renderer->reflectColor_uniform = glGetUniformLocation(renderer->standardShader.program, "reflectColor");
	renderer->roughness_uniform = glGetUniformLocation(renderer->standardShader.program, "roughness");
	renderer->curveShape_uniform = glGetUniformLocation(renderer->standardShader.program, "curveShape");
	renderer->_0degRef_uniform = glGetUniformLocation(renderer->standardShader.program, "_0degRef");
	renderer->_90degRef_uniform = glGetUniformLocation(renderer->standardShader.program, "_90degRef");
	renderer->bumpLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "bumpLevel");
	renderer->selfIllumLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "selfIllumLevel");
	renderer->selfIllumColor_uniform = glGetUniformLocation(renderer->standardShader.program, "selfIllumColor");
	renderer->rimLevel_uniform = glGetUniformLocation(renderer->standardShader.program, "rimLevel");
	renderer->rimColor_uniform = glGetUniformLocation(renderer->standardShader.program, "rimColor");
	renderer->rimPower_uniform = glGetUniformLocation(renderer->standardShader.program, "rimPower");
	renderer->tileUV_uniform = glGetUniformLocation(renderer->standardShader.program, "tileUV");
	renderer->offsetUV_uniform = glGetUniformLocation(renderer->standardShader.program, "offsetUV");

	renderer->diffuseMap_uniform = glGetUniformLocation(renderer->standardShader.program, "diffuseMap");
	renderer->normalMap_uniform = glGetUniformLocation(renderer->standardShader.program, "normalMap");
	renderer->mirrorMap_uniform = glGetUniformLocation(renderer->standardShader.program, "mirrorMap");
	renderer->envMap_uniform = glGetUniformLocation(renderer->standardShader.program, "envMap");
	renderer->roughnessMap_uniform = glGetUniformLocation(renderer->standardShader.program, "roughnessMap");
	
	//Particle Shader - Primary
	renderer->colorParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "color");
	renderer->mainTextureParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "mainTexture");

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
	renderer->lightPosStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "lightPos");
	renderer->lightColorStandard_uniform = glGetUniformLocation(renderer->standardShader.program, "lightColor");

	//Particle Shader - World Info
	renderer->modelToWorldParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "ModelToWorld");
	renderer->worldToViewParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "WorldToView");
	renderer->viewToProjectionParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "ViewToProjection");
	renderer->normalToWorldParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "NormalToWorld");

	renderer->cameraPosParticle_uniform = glGetUniformLocation(renderer->particleShader.program, "cameraPos");

	//Overlay Shader - World Info
	renderer->aspectRatio_uniform = glGetUniformLocation(renderer->overlayShader.program, "AspectRatio");
	renderer->modelToWorldOverlay_uniform = glGetUniformLocation(renderer->overlayShader.program, "ModelToWorld");

	//Skybox Shader - World Info
	renderer->modelToWorldSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "ModelToWorld");
	renderer->worldToViewSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "WorldToView");
	renderer->viewToProjectionSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "ViewToProjection");
	renderer->normalToWorldSkybox_uniform = glGetUniformLocation(renderer->skyboxShader.program, "NormalToWorld");
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
		if (i != MAP_ENV && i != MAP_COLOR_BUFFER && i != MAP_POSITION_BUFFER)//Bind images to GL_TEXTURE_2D
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
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
				cout << "Flumpty failed to load texture at: " + textureFilePaths[i] << endl;
			stbi_image_free(data);
		}
		else if(i == MAP_COLOR_BUFFER || i == MAP_POSITION_BUFFER)//These are for the Post Process Effects
		{
			glGenTextures(1, &texID);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Camera::WIDTH, Camera::HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glBindImageTexture(i, texID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
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
		}
	}
}

void Renderer::RenderScene(Renderer *renderer)
{
	// clear screen and depth buffers
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(renderer->geometry.vertexArray);

	//DRAW SKYBOX FIRST AS BACKGROUND
	glUseProgram(renderer->skyboxShader.program);

	//Program Post-Process Uniforms
	GLint screenDims_uniform = glGetUniformLocation(renderer->skyboxShader.program, "screenDims");
	GLint colorBufferMap_uniform = glGetUniformLocation(renderer->skyboxShader.program, "colorBufferMap");
	GLint positionBufferMap_uniform = glGetUniformLocation(renderer->skyboxShader.program, "positionBufferMap");
	GLint colorBuffer_uniform = glGetUniformLocation(renderer->skyboxShader.program, "colorBuffer");
	GLint positionBuffer_uniform = glGetUniformLocation(renderer->skyboxShader.program, "positionBuffer");

	glUniform2f(screenDims_uniform, (float)Camera::WIDTH, (float)Camera::HEIGHT);
	glUniform1i(colorBufferMap_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBufferMap_uniform, MAP_POSITION_BUFFER);
	glUniform1i(colorBuffer_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBuffer_uniform, MAP_POSITION_BUFFER);
	{
		//Get skybox
		Mesh mesh = Game::skybox.mesh;
		ParticleOverlayMaterial skyBoxMat = Game::skybox.particleOverlayMat;
		renderer->geometry.elementCount = mesh.indices.size();

		//Program uniforms
		glUniform4fv(renderer->colorSkybox_uniform, 1, value_ptr(skyBoxMat.color));
		glUniform1i(renderer->envMapSkybox_uniform, skyBoxMat.mainTexture);

		glUniformMatrix4fv(renderer->modelToWorldSkybox_uniform, 1, GL_TRUE, value_ptr(Game::skybox.GetModelToWorld()));
		glUniformMatrix4fv(renderer->normalToWorldSkybox_uniform, 1, GL_TRUE, value_ptr(Game::skybox.GetNormalToWorld()));
		//TODO strip out position from camera's WorldToView so the skybox appears at an 'infinite distance'
		mat4 worldToView;
		switch (renderer->camera.transform.rendertype) 
		{
		case RenderTypes::RT_EULER:
			worldToView = renderer->camera.GetWorldToViewMatrix();
			break;
		case RenderTypes::RT_QUAT:
			worldToView = renderer->camera.GetQuatWorldToViewMatrix();
			break;
		default:
			cout << "RenderTypes error" << endl;
			break;
		}

		glUniformMatrix4fv(renderer->worldToViewSkybox_uniform, 1, GL_TRUE, value_ptr(worldToView));
		glUniformMatrix4fv(renderer->viewToProjectionSkybox_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetViewToProjectionMatrix()));
		
		BufferGeoData(renderer, mesh);
		glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

	//DRAW WORLD OBJECTS NEXT
	glUseProgram(renderer->standardShader.program);

	//Program Post-Process Uniforms
	screenDims_uniform = glGetUniformLocation(renderer->standardShader.program, "screenDims");
	colorBufferMap_uniform = glGetUniformLocation(renderer->standardShader.program, "colorBufferMap");
	positionBufferMap_uniform = glGetUniformLocation(renderer->standardShader.program, "positionBufferMap");
	colorBuffer_uniform = glGetUniformLocation(renderer->standardShader.program, "colorBuffer");
	positionBuffer_uniform = glGetUniformLocation(renderer->standardShader.program, "positionBuffer");

	glUniform2f(screenDims_uniform, (float)Camera::WIDTH, (float)Camera::HEIGHT);
	glUniform1i(colorBufferMap_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBufferMap_uniform, MAP_POSITION_BUFFER);
	glUniform1i(colorBuffer_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBuffer_uniform, MAP_POSITION_BUFFER);

	//Program Uniforms for WorldToView and ViewToProjection
	mat4 worldToView;
	switch (renderer->camera.transform.rendertype) 
	{
	case RenderTypes::RT_EULER:
		worldToView = renderer->camera.GetWorldToViewMatrix();
		break;
	case RenderTypes::RT_QUAT:
		worldToView = renderer->camera.GetQuatWorldToViewMatrix();
		break;
	default:
		cout << "RenderTypes error" << endl;
		break;
	}
	glUniformMatrix4fv(renderer->worldToViewStandard_uniform, 1, GL_TRUE, value_ptr(worldToView));
	glUniformMatrix4fv(renderer->viewToProjectionStandard_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetViewToProjectionMatrix()));

	//For each World GameObject Load geometry into GPU, bind and draw it
	for (int i = 0; i < Game::worldObjectList.size(); i++)
	{
		//Setup drawing configs
		GameObject gameObject = Game::worldObjectList[i];
		Mesh mesh = gameObject.mesh;
		StandardMaterial standardMat = gameObject.standardMat;
		renderer->geometry.elementCount = mesh.indices.size();

		//PROGRAM UNIFORMS - PRIMARY
		glUniform1f(renderer->diffuseLevel_uniform, standardMat.diffuseLevel);
		glUniform3fv(renderer->diffuseColor_uniform, 1, value_ptr(standardMat.diffuseColor));
		glUniform1f(renderer->reflectivity_uniform, standardMat.reflectivity);
		glUniform3fv(renderer->reflectColor_uniform, 1, value_ptr(standardMat.reflectColor));
		glUniform1f(renderer->roughness_uniform, standardMat.roughness);
		glUniform1f(renderer->curveShape_uniform, standardMat.curveShape);
		glUniform1f(renderer->_0degRef_uniform, standardMat._0degRef);
		glUniform1f(renderer->_90degRef_uniform, standardMat._90degRef);
		glUniform1f(renderer->bumpLevel_uniform, standardMat.bumpLevel);
		glUniform1f(renderer->selfIllumLevel_uniform, standardMat.selfIllumLevel);
		glUniform3fv(renderer->selfIllumColor_uniform, 1, value_ptr(standardMat.selfIllumColor));

		glUniform1f(renderer->rimLevel_uniform, standardMat.rimLevel);
		glUniform3fv(renderer->rimColor_uniform, 1, value_ptr(standardMat.rimColor));
		glUniform1f(renderer->rimPower_uniform, standardMat.rimPower);

		glUniform1i(renderer->diffuseMap_uniform, standardMat.diffuseMap);
		glUniform1i(renderer->normalMap_uniform, standardMat.normalMap);
		glUniform1i(renderer->mirrorMap_uniform, standardMat.mirrorMap);
		glUniform1i(renderer->envMap_uniform, standardMat.envMap);
		glUniform1i(renderer->roughnessMap_uniform, standardMat.roughnessMap);

		glUniform2fv(renderer->tileUV_uniform, 1, value_ptr(standardMat.tileUV));
		glUniform2fv(renderer->offsetUV_uniform, 1, value_ptr(standardMat.offsetUV));

		mat4 modelToWorld;
		mat4 normalToWorld;

		//PROGRAM UNIFORMS - WORLD INFO
		switch (gameObject.transform.rendertype)
		{
		case RenderTypes::RT_EULER:
			modelToWorld = gameObject.GetModelToWorld();
			normalToWorld = gameObject.GetNormalToWorld();	
			break;
		case RenderTypes::RT_QUAT:
			modelToWorld = gameObject.transform.GetQuatModelToWorld();
			normalToWorld = gameObject.transform.GetQuatNormalToWorld();
			break;
		default:
			cout << "GameObject RenderTypes error" << endl;
			break;
		}

		glUniformMatrix4fv(renderer->modelToWorldStandard_uniform, 1, GL_TRUE, value_ptr(modelToWorld));
		glUniformMatrix4fv(renderer->normalToWorldStandard_uniform, 1, GL_TRUE, value_ptr(normalToWorld));
		

		glUniform3fv(renderer->cameraPosStandard_uniform, 1, value_ptr(renderer->camera.transform.position));
		glUniform4fv(renderer->lightPosStandard_uniform, 1, value_ptr(vec4(7, 10, -7, 1)));
		glUniform4fv(renderer->lightColorStandard_uniform, 1, value_ptr(vec4(1, 1, 1, 1)));

		BufferGeoData(renderer, mesh);
		glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

	//RETAIN DEPTH BUFFER AND DRAW PARTICLE SYSTEM OBJECTS
	glUseProgram(renderer->particleShader.program);

	//Program Post-Process Uniforms
	screenDims_uniform = glGetUniformLocation(renderer->particleShader.program, "screenDims");
	colorBufferMap_uniform = glGetUniformLocation(renderer->particleShader.program, "colorBufferMap");
	positionBufferMap_uniform = glGetUniformLocation(renderer->particleShader.program, "positionBufferMap");
	colorBuffer_uniform = glGetUniformLocation(renderer->particleShader.program, "colorBuffer");
	positionBuffer_uniform = glGetUniformLocation(renderer->particleShader.program, "positionBuffer");

	glUniform2f(screenDims_uniform, (float)Camera::WIDTH, (float)Camera::HEIGHT);
	glUniform1i(colorBufferMap_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBufferMap_uniform, MAP_POSITION_BUFFER);
	glUniform1i(colorBuffer_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBuffer_uniform, MAP_POSITION_BUFFER);

	//Program Uniforms for WorldToView and ViewToProjection
	glUniformMatrix4fv(renderer->worldToViewParticle_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetWorldToViewMatrix()));
	glUniformMatrix4fv(renderer->viewToProjectionParticle_uniform, 1, GL_TRUE, value_ptr(renderer->camera.GetViewToProjectionMatrix()));

	//For each ParticleSystem GameObject Load each particle's geometry into GPU, bind and draw it
	for (int i = 0; i < Game::particleObjectList.size(); i++)
	{
		//Setup drawing configs
		GameObject gameObject = Game::particleObjectList[i];

		//TODO Get meshes of individual particles. Likely planes

		ParticleOverlayMaterial particleOverlayMat = gameObject.particleOverlayMat;
		
		//TODO Get element count of particle. Same for each particle in a given ParticleSystem.

		//PROGRAM UNIFORMS - PRIMARY
		glUniform4fv(renderer->colorParticle_uniform, 1, value_ptr(particleOverlayMat.color));
		glUniform1f(renderer->reflectivity_uniform, particleOverlayMat.mainTexture);

		//PROGRAM UNIFORMS - WORLD INFO
		mat4 modelToWorld = gameObject.GetModelToWorld();
		mat4 normalToWorld = gameObject.GetNormalToWorld();
		glUniformMatrix4fv(renderer->modelToWorldParticle_uniform, 1, GL_TRUE, value_ptr(modelToWorld));
		glUniformMatrix4fv(renderer->normalToWorldParticle_uniform, 1, GL_TRUE, value_ptr(normalToWorld));

		glUniform3fv(renderer->cameraPosParticle_uniform, 1, value_ptr(renderer->camera.transform.position));

		//TODO fashion loop to draw all particles within this ParticleSystem
	}

	//CLEAR DEPTH BUFFER AND DRAW POST-PROCESS RECT
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderer->fsppShader.program);
	//Program Post-Process Uniforms
	screenDims_uniform = glGetUniformLocation(renderer->fsppShader.program, "screenDims");
	colorBuffer_uniform = glGetUniformLocation(renderer->fsppShader.program, "colorBufferMap");
	positionBuffer_uniform = glGetUniformLocation(renderer->fsppShader.program, "positionBufferMap");

	glUniform2f(screenDims_uniform, (float)Camera::WIDTH, (float)Camera::HEIGHT);
	glUniform1i(colorBuffer_uniform, MAP_COLOR_BUFFER);
	glUniform1i(positionBuffer_uniform, MAP_POSITION_BUFFER);
	{
		Mesh mesh = GeoGenerator::MakeRect(2.0, 2.0, GA_CENTER);
		renderer->geometry.elementCount = mesh.indices.size();

		BufferGeoData(renderer, mesh);
		//glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

	//CLEAR DEPTH BUFFER AND DRAW OVERLAY OBJECTS
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderer->overlayShader.program);
	glUniform1f(renderer->aspectRatio_uniform, (float)Camera::WIDTH / (float)Camera::HEIGHT);

	//For each Overlay GameObject, Load geometry, bind and draw
	for (int i = 0; i < Game::overlayObjectList.size(); i++)
	{
		//Setup drawing configs
		GameObject gameObject = Game::overlayObjectList[i];
		Mesh mesh = gameObject.mesh;
		ParticleOverlayMaterial particleOverlayMat = gameObject.particleOverlayMat;
		renderer->geometry.elementCount = mesh.indices.size();

		//PROGRAM UNIFORMS - PRIMARY
		glUniform4fv(renderer->colorOverlay_uniform, 1, value_ptr(particleOverlayMat.color));
		glUniform1i(renderer->mainTextureOverlay_uniform, particleOverlayMat.mainTexture);

		//PROGRAM UNIFORMS - WORLD INFO
		mat4 modelToWorld = gameObject.GetModelToWorld();
		glUniformMatrix4fv(renderer->modelToWorldOverlay_uniform, 1, GL_TRUE, value_ptr(modelToWorld));

		BufferGeoData(renderer, mesh);
		glDrawElements(GL_TRIANGLES, renderer->geometry.elementCount, GL_UNSIGNED_INT, nullptr);
	}

	//Unbind geometry and shader
	glBindVertexArray(0);
	glUseProgram(0);

	// check for and report any OpenGL errors
	RendererUtility::CheckGLErrors();
}

Camera* Renderer::GetCamera(int index)
{
	return cameraList[index];
}

void Renderer::BufferGeoData(Renderer *renderer, Mesh mesh)
{
	//BUFFER GEOMETRY AND INDICIES
	//Buffer Positions
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh.positions.size(), mesh.positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Renderer::POSITION_INDEX);

	//Buffer Colors
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh.colors.size(), mesh.colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Renderer::COLOR_INDEX);

	//Buffer Normals
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Renderer::NORMAL_INDEX);

	//Buffer Texcoords
	glBindBuffer(GL_ARRAY_BUFFER, renderer->geometry.texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * mesh.texcoords.size(), mesh.texcoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Renderer::TEXCOORD_INDEX);

	//Buffer Indicies
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW);
}