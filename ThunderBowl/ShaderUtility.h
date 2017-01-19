#pragma once
#include "Structs.h"

class ShaderUtility
{
public:
	static void QueryGLVersion();
	static bool CheckGLErrors();
	static bool InitializeShaders(Shader *shader);

protected:

private:
	static string LoadSource(const string &filename);
	static GLuint CompileShader(GLenum shaderType, const string &source);
	static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};