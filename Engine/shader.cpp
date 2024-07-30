#include "shader.h"


ShaderSource ReadShaderCode(const char* filepath1, const char* filepath2) {
	std::ifstream ShaderFile1(filepath1);
	std::string ShaderCode1;
	std::string line1;
	while (std::getline(ShaderFile1, line1)) {
		ShaderCode1 += line1 + "\n";
	}
	ShaderFile1.close();
	std::ifstream ShaderFile2(filepath2);
	std::string ShaderCode2;
	std::string line2;
	while (std::getline(ShaderFile2, line2)) {
		ShaderCode2 += line2 + "\n";
	}
	printf("%s", ShaderCode2.c_str());
	ShaderFile2.close();
	return { ShaderCode1.c_str(), ShaderCode2.c_str() };
}

GLuint CompileShader(GLuint ShaderType, const std::string& ShaderSource) {
	GLuint ShaderID = glCreateShader(ShaderType);
	const char* src = ShaderSource.c_str();
	glShaderSource(ShaderID, 1, &src, NULL);
	glCompileShader(ShaderID);
	return ShaderID;
}

GLuint CreateShader(const std::string& VertexShader, const std::string& FragmentShader) {
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);
	// program won't be deleted until it is detached

	return program;
}