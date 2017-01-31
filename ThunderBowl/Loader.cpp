#include "Loader.h"

vector<Mesh>& Loader::getMeshes() {
	return meshes;
}


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
		getMeshes().push_back(processMesh(mesh, scene));

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
		glm::vec3 posVec;
		posVec.x = mesh->mVertices[i].x;
		posVec.y = mesh->mVertices[i].y;
		posVec.z = mesh->mVertices[i].z;
		loadedMesh.positions.push_back(posVec);

		glm::vec3 norVec;
		norVec.x = mesh->mNormals[i].x;
		norVec.y = mesh->mNormals[i].y;
		norVec.z = mesh->mNormals[i].z;
		loadedMesh.normals.push_back(norVec);

		glm::vec2 texVec;
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			texVec.x = mesh->mTextureCoords[0][i].x;
			texVec.y = mesh->mTextureCoords[0][i].y;
			loadedMesh.texcoords.push_back(texVec);
		}
		else {
			texVec = glm::vec2(0.0f, 0.0f);
			loadedMesh.texcoords.push_back(texVec);
		}
			
	}
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			loadedMesh.indices.push_back(face.mIndices[j]);
	}

	loadedMesh.elementCount = mesh->mNumVertices;

	return  loadedMesh;
}



