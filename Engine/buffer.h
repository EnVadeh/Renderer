#pragma once
#include "utils.h"
#include <iostream>
#include "texture.h"

struct vertex {
	glm::vec3 Position;
	glm::vec2 texCoord;
};

class MeshBuffer {
private:
	//GLuint VAOs[BufferAttribs::NumVAOs];
	GLuint VAO;
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

class FrameBuffer {
private: 
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLuint FBO;
	GLuint RBO;
	GLuint render_texture;
public: 
	GLuint setupFrameBuffer();
	GLuint renderTexture();
	void setupRenderBuffer();
	void readFromBuffer();
	void ActivateRenderTexture(GLuint shaderID);
};

//I am too tired ot dealing with the normal buffer class, I don't wanna waste time with that and get confused
class ScreenQuad {
private: 
	GLuint VAO;
	GLuint Buffers[BufferAttribs::NumBuffers];
public: 
	void drawQuad(GLuint shaderID);
};
