#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

GLint setUniform(GLuint shader, std::string uniformName);
glm::mat4 createGeometricToWorldMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

struct BufferAttribs {
	//enum VAO_IDs { Triangles, NumVAOs };
	enum Buffer_IDs { ArrayBuffer, ElementBuffer, NumBuffers };
	enum Attrib_IDs { vPos, vTex, vNormal, vTangent, vBiTangent };
	//GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
};

static const std::string RenderTextureNames[3] = {
    "colorRT",
    "normalRT",
    "depthRT"
};




