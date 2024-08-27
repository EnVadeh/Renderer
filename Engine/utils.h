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
	enum SSBuffers { vAlbedo, NumSSBs};
	enum SSBsize{ VertexSSBO =  16000000, floatSSBO = 4000000, vec3SSBO = 12000000}; //i shoudl probably get size for cstom like if 1000 vertices: 1000 * sizeof(glm::vec4) type shit
	//GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
};

struct Materials {
	glm::vec4 Albedo; //alignment 16, size 16
	GLfloat Mettalic; //alignment 4, size 4
	GLfloat Roughness; //alignment 4, size 4
	GLfloat padding; //alignment 4, size 4
	GLfloat paddin2; //alignment 4, size 4
}; //total alignment 16, size should be mulitple of 16, so 16 + 4 + 4 is 24. so need 8 to reach 32!

static const std::string RenderTextureNames[3] = {
    "colorRT",
    "normalRT",
    "depthRT"
};