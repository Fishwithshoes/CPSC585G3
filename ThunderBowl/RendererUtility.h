#pragma once
#include "Structs.h"

class RendererUtility
{
public:
	static void QueryGLVersion();
	static bool CheckGLErrors();
	static void ErrorCallback(int error, const char* description);
	static bool InitializeShader(Shader *shader, const string &vertSource, const string &fragSource);
	static bool InitializeGeometry(Geometry *geometry);

protected:

private:
	static string LoadSource(const string &filename);
	static GLuint CompileShader(GLenum shaderType, const string &source);
	static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};