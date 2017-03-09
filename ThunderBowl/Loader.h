#pragma once
#include "Structs.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Loader
{
public:


	//string directory;

	void loadModel(string path, vec3 scale, bool insideOut);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Mesh>& getMeshes();

private:
	vector<Mesh> meshes;

	vec3 scalar;
	bool flipMesh;
};

