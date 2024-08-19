#pragma once
#include "utils.h"
#include <iostream>

struct vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 texCoord;
};



class MeshBuffer {
private:
	//GLuint VAOs[BufferAttribs::NumVAOs];
	GLuint VAO;
	GLuint Buffers[BufferAttribs::NumBuffers];
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> Pos;
	std::vector<glm::vec3> Size;
	bool samesize;
	
public:
	void setVertices(std::vector<vertex> vertices, std::vector<unsigned int> indices);
	void DrawCall(GLuint shaderID);
};

