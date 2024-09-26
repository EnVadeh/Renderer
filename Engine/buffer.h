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
	std::string name = "texture_diffuse";
	texture terrain;
public:
	TerrainBuffer(size_t width, size_t height, std::vector<std::string> texName);
	void TerrainDraw(GLuint shaderID, GLuint shadowID);
	
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
	void setupRenderBuffer(); //for depth and stencil shit
	void readFromBuffer();
	void ActivateRenderTexture(GLuint shaderID);
};

class SSBufferObject {
private:
	GLuint SSBO[BufferAttribs::NumSSBs];
	GLuint SSBO_sizes[BufferAttribs::NumSSBs] = { BufferAttribs::vec3SSBO };
	std::vector<Materials> Data;
	
public:
	SSBufferObject(std::vector<Materials> Temp);
	void BindSSBOs();
	void UnbindSSBOs();
	GLuint SSBOid(size_t index); //use this to manually give data to some SSBO from the applicaton
};

class ShadowMap {
private:
	GLuint FBO;
	GLuint shadowRT;
public:
	GLuint setupShadowFB();
	void activateshadowRT(GLuint shaderID);
	GLuint returnShadowRT();
};

//I am too tired ot dealing with the normal buffer class, I don't wanna waste time with that and get confused
class ScreenQuad {
private: 
	GLuint VAO;
	GLuint Buffers[BufferAttribs::NumBuffers];
public: 
	void drawQuad(GLuint shaderID);
};

class skyBuffer {
private:
	GLuint VAO;
	GLuint Buffer;
	GLfloat data[108];
public:
	skyBuffer();
	void draw();
};

void useFB(GLuint FB);
void useSB(GLuint SB);
void bindFB(GLuint FB);