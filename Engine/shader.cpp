#include "shader.h"
#include <vector>
#include <iostream>

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

	GLint infoLogLength = 512;
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// program won't be deleted until it is detached

	return program;
}

TesselationSource ReadTessShaderSource(const char* filepath1, const char* filepath2, const char* filepath3, const char* filepath4) {
	std::ifstream ShaderFile1(filepath1); 
	std::string ShaderCode1;
	std::string line1;
	while (std::getline(ShaderFile1, line1)) {
		ShaderCode1 += line1 + "\n"; //vs
	}
	ShaderFile1.close();
	std::ifstream ShaderFile2(filepath2);
	std::string ShaderCode2;
	std::string line2;
	while (std::getline(ShaderFile2, line2)) {
		ShaderCode2 += line2 + "\n"; //tcs
	}
	ShaderFile2.close();
	std::ifstream ShaderFile3(filepath3);
	std::string ShaderCode3;
	std::string line3;
	while (std::getline(ShaderFile3, line3)) {
		ShaderCode3 += line3 + "\n"; //tes
	}
	ShaderFile3.close();
	std::ifstream ShaderFile4(filepath4);
	std::string ShaderCode4;
	std::string line4;
	while (std::getline(ShaderFile4, line4)) {
		ShaderCode4 += line4 + "\n"; //fs
	}
	ShaderFile4.close();
	return { ShaderCode1.c_str(), ShaderCode2.c_str(), ShaderCode3.c_str(), ShaderCode4.c_str() };
}


GLuint CreateTessShader(const std::string& VertexShader, const std::string& TessControlShader, const std::string& TessEvalShader, const std::string& FragmentShader) {
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	GLuint tcs = CompileShader(GL_TESS_CONTROL_SHADER, TessControlShader);
	GLuint tes = CompileShader(GL_TESS_EVALUATION_SHADER, TessEvalShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);


	
	return program;
}