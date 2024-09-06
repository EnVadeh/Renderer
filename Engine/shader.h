#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

struct TesselationSource {
	std::string VertexSource;
	std::string TessellationControlSource;
	std::string TessellationEvaluationSource;
	std::string FragmentSource;
};

ShaderSource ReadShaderCode(const char* filepath1, const char* filepath2);

GLuint CompileShader(GLuint ShaderType, const std::string& ShaderSource);

GLuint CreateShader(const std::string& VertexShader, const std::string& FragmentShader);

TesselationSource ReadTessShaderSource(const char* filepath1, const char* filepath2, const char* filepath3, const char* filepath4);

GLuint CreateTessShader(const std::string& VertexShader, const std::string& TessControlShader, const std::string& TessEvalShader, const std::string& FragmentShader);