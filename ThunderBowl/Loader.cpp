#include "Loader.h"


void Loader::loadModel(string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	//directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Loader::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Loader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh loadedMesh;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		loadedMesh.positions[i].x = mesh->mVertices[i].x;
		loadedMesh.positions[i].y = mesh->mVertices[i].y;
		loadedMesh.positions[i].z = mesh->mVertices[i].z;

		loadedMesh.normals[i].x = mesh->mNormals[i].x;
		loadedMesh.normals[i].y = mesh->mNormals[i].y;
		loadedMesh.normals[i].z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			loadedMesh.texcoords[i].x = mesh->mTextureCoords[0][i].x;
			loadedMesh.texcoords[i].y = mesh->mTextureCoords[0][i].y;
		}
		else
			loadedMesh.texcoords[i] = glm::vec2(0.0f, 0.0f);
	}
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			loadedMesh.indices.push_back(face.mIndices[j]);
	}

	return  loadedMesh;
}



