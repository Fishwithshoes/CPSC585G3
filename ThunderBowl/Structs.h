#pragma once

#include "CommonIncludes.h"

using namespace glm;
using namespace std;

struct Mesh
{
	vector<vec3> positions;
	vector<vec3> colors;
	vector<vec3> normals;
	vector<vec2> texcoords;
	GLsizei elementCount;
	vector<GLuint> indicies;
};

struct Shader
{
	GLuint vertex;
	GLuint fragment;
	GLuint program;
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
