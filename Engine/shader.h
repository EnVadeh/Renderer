#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderSource ReadShaderCode(const char* filepath1, const char* filepath2);

GLuint CompileShader(GLuint ShaderType, const std::string& ShaderSource);

GLuint CreateShader(const std::string& VertexShader, const std::string& FragmentShader);