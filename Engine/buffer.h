#pragma once
#include "utils.h"
#include <iostream>

struct vertex {
	glm::vec3 Position;
};

struct BufferAttribs {
	enum VAO_IDs { Triangles, NumVAOs };
	enum Buffer_IDs { ArrayBuffer, TextureBuffer, NumBuffers};
	enum Attrib_IDs { vPos, vTex};
	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
};

class MeshBuffer {
private:
	GLuint VAOs[BufferAttribs::NumVAOs];
	GLuint Buffers[BufferAttribs::NumBuffers];
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> texCoord;
	std::vector<glm::vec3> Pos;
	std::vector<glm::vec3> Size;
	bool samesize;
	
public:
	void setVertices(GLfloat vertices[], GLfloat texCoords[], GLfloat Pos[], GLfloat Size[], GLuint shaderID, bool size, int number);
	void DrawCall(GLuint shaderID, int number);
};

