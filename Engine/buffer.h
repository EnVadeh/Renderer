#pragma once
#include "utils.h"
#include <iostream>
#include "texture.h"

struct vertex {
	glm::vec3 Position;
	glm::vec2 texCoord;
	glm::vec3 Normal;
};

class TerrainBuffer {
private:
	//GLuint VAOs[BufferAttribs::NumVAOs];
	GLuint VAO;
	GLuint Buffers[BufferAttribs::NumBuffers];
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	size_t terrainW, terrainH;
	std::string name = "terrain";
	texture terrain;
public:
	TerrainBuffer(size_t width, size_t height, std::vector<std::string> texName);
	void TerrainDraw(GLuint shaderID);
	
};

class FrameBuffer {
private: 
	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	GLuint FBO;
	GLuint RBO;
	std::vector<GLuint> render_texture;
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
